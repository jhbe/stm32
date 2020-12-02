#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <libusb-1.0/libusb.h>
#include <math.h>

#define VENDOR_ID   0x3456
#define PRODUCT_ID  0x1150

bool g_verbose = true;
libusb_device_handle *device_handle = NULL;
unsigned char out_endpoint;
unsigned char in_endpoint;

bool Open() {
    libusb_device **list = NULL;
    libusb_device *device = NULL;
    libusb_config_descriptor *config_descriptor = NULL;

    int result;
    bool success = false;

    do {
        if ((result = libusb_init(NULL)) != LIBUSB_SUCCESS) {
            printf("Failed to initialise libusb; %s.\n",
                   libusb_error_name(result));
            break;
        }

        int number_of_devices = libusb_get_device_list(NULL, &list);
        if (number_of_devices < 1) {
            printf("There are no devices!.\n");
            return -1;
        }

        for (int i = 0; i < number_of_devices; i++) {
            libusb_device_descriptor device_descriptor;
            if ((result = libusb_get_device_descriptor(list[i],
                                                       &device_descriptor)) != LIBUSB_SUCCESS) {
                printf("Failed to get the device descriptor; %s\n",
                       libusb_error_name(result));
                break;
            }

            printf("VID: %04x, PID:%04x\n",
                   device_descriptor.idVendor,
                   device_descriptor.idProduct);

            if (device_descriptor.idVendor == VENDOR_ID
                && device_descriptor.idProduct == PRODUCT_ID) {
                printf("Found the device.\n");
                device = list[i];
            }
        }

        if (device == NULL) {
            break;
        }

        if ((result = libusb_open(device, &device_handle)) != LIBUSB_SUCCESS) {
            printf("Failed to open the device; %s.\n",
                   libusb_error_name(result));
            break;
        }

        if ((result = libusb_get_active_config_descriptor(device, &config_descriptor)) != LIBUSB_SUCCESS) {
            printf("Failed to get the config descriptor; %s\n",
                   libusb_error_name(result));
            break;
        }

        if (config_descriptor->bNumInterfaces != 1) {
            printf("Odd, this USB device supports %i interfaces. Expected one.\n", config_descriptor->bNumInterfaces);
            break;
        }

        const libusb_interface *interface = config_descriptor->interface;
        if (interface->num_altsetting != 1) {
            printf("Odd, this USB interface supports %i alt interfaces. Expected one.\n", interface->num_altsetting);
            break;
        }

        const libusb_interface_descriptor *interface_descriptor = interface->altsetting;
        const libusb_endpoint_descriptor *endpoint_descriptor = interface_descriptor->endpoint;
        for (int i = 0; i < interface_descriptor->bNumEndpoints; i++) {
            if ((endpoint_descriptor->bEndpointAddress & 0x80) == LIBUSB_ENDPOINT_IN) {
                in_endpoint = endpoint_descriptor->bEndpointAddress;
            }
            if ((endpoint_descriptor->bEndpointAddress & 0x80) == LIBUSB_ENDPOINT_OUT) {
                out_endpoint = endpoint_descriptor->bEndpointAddress;
            }

            endpoint_descriptor++;
        }

        success = true;
    } while (false);

    if (config_descriptor != NULL) {
        libusb_free_config_descriptor(config_descriptor);
        config_descriptor = NULL;
    }

    if (list != NULL) {
        libusb_free_device_list(list, 1);
        list = NULL;
    }

    return success;
}

void Close() {
    if (device_handle != NULL) {
        libusb_close(device_handle);
        device_handle = NULL;
    }

    libusb_exit(NULL);
}

bool Send(unsigned char *buffer, int length) {
    int result;
    int bytes_written;
    if ((result = libusb_bulk_transfer(device_handle, out_endpoint, buffer, length, &bytes_written, 1000)) !=
        LIBUSB_SUCCESS) {
        if (g_verbose) {
            printf("libusb_bulk_transfer failed; %s\n", libusb_error_name(result));
        }
        return false;
    }
    if (length != bytes_written && g_verbose) {
        printf("libusb_bulk_transfer worked, but only wrote %i of %i bytes\n",
               bytes_written, length);
        return false;
    }

    return true;
}

bool Send(unsigned char byte) {
    return Send(&byte, 1);
}

int counter = 0;
bool Receive(unsigned char *buffer, int *length) {
    memset(buffer, 0xcd, *length);

    int result;
    int bytes_read;
    if ((result = libusb_bulk_transfer(device_handle, in_endpoint, buffer, *length, &bytes_read, 1000)) !=
        LIBUSB_SUCCESS) {
        if (g_verbose) {
            printf("libusb_bulk_transfer failed; %s\n", libusb_error_name(result));
        }
        usleep(500000);
        *length = 0;
        return false;
    }

    *length = bytes_read;
    return true;
}

void *rx(void *ptr) {
    int counter = 0;
    while (1) {
        unsigned char buffer[64];
        int length = sizeof(buffer);
        if (Receive(buffer, &length)) {
/*            float *acc = (float *)buffer;
            double sum = sqrt(x*x+y*y+z*z);
            printf("%i % 2.3f % 2.3f % 2.3f % 2.3f \n", counter++, acc[0], acc[1], acc[2], sum);

            printf("%i ", length);
            for (int i = 0; i < length; i++) {
                printf("%02x ", buffer[i]);
            }
            printf("\n");
*/
//            printf("%i 0x%02x 0x%02x\n", length, buffer[0], buffer[1]);

           buffer[length] = 0;
           printf("% 10i %i: %s\n", counter++, length, buffer);
        }
    }
}

int main() {
    Open();

    pthread_t rxThread;
    int thread = pthread_create(&rxThread, NULL, rx, NULL);

    while (1) {
        usleep(300000);
    }

    Close();
}
