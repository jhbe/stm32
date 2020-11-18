#include "stm32f4xx.h"
#include "../../common/delay.h"
#include <usb.h>
#include <stdio.h>
#include <string.h>

usbd_device usbdDevice;

#define EP1_OUT (0x01 | USB_EPDIR_OUT)
#define EP1_IN  (0x01 | USB_EPDIR_IN)

#define EP0_SIZE 64
#define EP1_SIZE 64
uint32_t ep0Buffer[EP0_SIZE];
uint32_t ep1Buffer[EP1_SIZE];

void OTG_FS_IRQHandler(void) {
    usbd_poll(&usbdDevice);
}

struct std_config {
    struct usb_config_descriptor config;
    struct usb_interface_descriptor interface;
    struct usb_endpoint_descriptor ep0out;
    struct usb_endpoint_descriptor ep0in;
};

static struct usb_device_descriptor deviceDescriptor = {
        .bLength            = sizeof(struct usb_device_descriptor),
        .bDescriptorType    = USB_DTYPE_DEVICE,
        .bcdUSB             = VERSION_BCD(2, 0, 0),
        .bDeviceClass       = USB_CLASS_PER_INTERFACE,
        .bDeviceSubClass    = USB_SUBCLASS_NONE,
        .bDeviceProtocol    = USB_PROTO_NONE,
        .bMaxPacketSize0    = EP0_SIZE,
        .idVendor           = 0x3456,
        .idProduct          = 0x1150,
        .bcdDevice          = VERSION_BCD(1, 0, 0),
        .iManufacturer      = 1,
        .iProduct           = 2,
        .iSerialNumber      = NO_DESCRIPTOR,
        .bNumConfigurations = 1,
};

static struct std_config config_desc = {
        .config = {
                .bLength                = sizeof(struct usb_config_descriptor),
                .bDescriptorType        = USB_DTYPE_CONFIGURATION,
                .wTotalLength           = sizeof(struct std_config),
                .bNumInterfaces         = 1,
                .bConfigurationValue    = 1,
                .iConfiguration         = NO_DESCRIPTOR,
                .bmAttributes           = USB_CFG_ATTR_RESERVED,
                .bMaxPower              = USB_CFG_POWER_MA(100),
        },
        .interface = {
                .bLength                = sizeof(struct usb_interface_descriptor),
                .bDescriptorType        = USB_DTYPE_INTERFACE,
                .bInterfaceNumber       = 0,
                .bAlternateSetting      = 0,
                .bNumEndpoints          = 2,
                .bInterfaceClass        = USB_CLASS_VENDOR,
                .bInterfaceSubClass     = USB_SUBCLASS_VENDOR,
                .bInterfaceProtocol     = USB_PROTO_VENDOR,
                .iInterface             = NO_DESCRIPTOR,
        },
        .ep0out = {
                .bLength                = sizeof(struct usb_endpoint_descriptor),
                .bDescriptorType        = USB_DTYPE_ENDPOINT,
                .bEndpointAddress       = EP1_OUT,
                .bmAttributes           = USB_EPTYPE_BULK,
                .wMaxPacketSize         = EP1_SIZE,
                .bInterval              = 0x01,
        },
        .ep0in = {
                .bLength                = sizeof(struct usb_endpoint_descriptor),
                .bDescriptorType        = USB_DTYPE_ENDPOINT,
                .bEndpointAddress       = EP1_IN,
                .bmAttributes           = USB_EPTYPE_BULK,
                .wMaxPacketSize         = EP1_SIZE,
                .bInterval              = 0x01,
        }
};

static struct usb_string_descriptor langDesc = USB_ARRAY_DESC(USB_LANGID_ENG_US);
static struct usb_string_descriptor manufDesc = USB_STRING_DESC("Johan");
static struct usb_string_descriptor prodDesc = USB_STRING_DESC("F405 UsbTest");

static struct usb_string_descriptor *const stringTable[] = {
        &langDesc,
        &manufDesc,
        &prodDesc,
};

int johan = 0;

void onEp1(usbd_device *dev, uint8_t event, uint8_t ep) {
    GPIO_ToggleBits(GPIOA, GPIO_Pin_14);

    switch (event) {
        case usbd_evt_eprx: {
            char rxBuffer[64];
            int length = usbd_ep_read(dev, EP1_OUT, rxBuffer, 64);
            rxBuffer[length] = 0;

            char txBuffer[64];
            sprintf(txBuffer, "jhbe \"%s\" %i", rxBuffer, johan++);
            usbd_ep_write(dev, EP1_IN, txBuffer, strlen(txBuffer));
            if (johan > 1000) {
                johan = 0;
            }
        }
    }
}

usbd_respond onConfig(usbd_device *dev, uint8_t cfg) {
    switch (cfg) {
        case 0: // Deconfiguring device
            usbd_ep_deconfig(dev, EP1_OUT);
            //usbd_ep_deconfig(dev, EP1_IN);
            usbd_reg_endpoint(dev, EP1_OUT, 0);
            usbd_reg_endpoint(dev, EP1_IN, 0);
            break;

        case 1: // Configuring device
            usbd_reg_endpoint(dev, EP1_OUT, onEp1);
            // usbd_reg_endpoint(dev, EP1_IN, onEp1);
            usbd_ep_config(dev, EP1_OUT, USB_EPTYPE_BULK, EP1_SIZE);
            usbd_ep_config(dev, EP1_IN, USB_EPTYPE_BULK, EP1_SIZE);
            break;
        default:
            return usbd_fail;
    }
    return usbd_ack;
}

usbd_respond onDescriptor(usbd_ctlreq *req, void **address, uint16_t *length) {
    const uint8_t type = req->wValue >> 8;
    const uint8_t number = req->wValue & 0xFF;

    switch (type) {
        case USB_DTYPE_DEVICE:
            *address = &deviceDescriptor;
            *length = sizeof(deviceDescriptor);
            break;

        case USB_DTYPE_CONFIGURATION:
            *address = &config_desc;
            *length = sizeof(config_desc);
            break;

        case USB_DTYPE_STRING:
            if (number < 3) {
                *address = stringTable[number];
                *length = ((struct usb_header_descriptor *) stringTable[number])->bLength;
            } else {
                return usbd_fail;
            }
            break;
    }

    return usbd_ack;
}

int main() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef gis;
    gis.GPIO_Mode = GPIO_Mode_OUT;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_Speed = GPIO_Speed_100MHz;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gis.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOA, &gis);
    gis.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &gis);

    /*
     * Set the SysTick to tick once per millisecond.
     */
    SysTick_Config(SystemCoreClock / 1000);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* Configure SOF ID DM DP Pins */
    gis.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;

    gis.GPIO_Speed = GPIO_Speed_100MHz;
    gis.GPIO_Mode = GPIO_Mode_AF;
    gis.GPIO_OType = GPIO_OType_PP;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_OTG1_FS);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_OTG1_FS);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_OTG1_FS);

    /* Configure VBUS Pin */
    gis.GPIO_Pin = GPIO_Pin_9;
    gis.GPIO_Speed = GPIO_Speed_100MHz;
    gis.GPIO_Mode = GPIO_Mode_IN;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gis);

    /* Configure ID pin */
    gis.GPIO_Pin = GPIO_Pin_10;
    gis.GPIO_OType = GPIO_OType_OD;
    gis.GPIO_PuPd = GPIO_PuPd_UP;
    gis.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &gis);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_OTG1_FS);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitTypeDef nis;
    nis.NVIC_IRQChannel = OTG_FS_IRQn;
    nis.NVIC_IRQChannelPreemptionPriority = 1;
    nis.NVIC_IRQChannelSubPriority = 3;
    nis.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nis);

    usbd_init(&usbdDevice, &usbd_hw, EP0_SIZE, ep0Buffer, EP0_SIZE);
    usbd_reg_config(&usbdDevice, onConfig);
    usbd_reg_descr(&usbdDevice, onDescriptor);
    usbd_enable(&usbdDevice, true);
    usbd_connect(&usbdDevice, true);

    while (1) {
        Delay(1000);
        usbd_ep_write(&usbdDevice, EP1_IN, "JHBE", 4);
        GPIO_ToggleBits(GPIOB, GPIO_Pin_9);
    }
}

void assert_failed(uint8_t *file, uint32_t line) {
    while (1);
}
