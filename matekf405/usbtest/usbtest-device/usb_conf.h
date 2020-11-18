#ifndef VMG_USB_CONF_H
#define VMG_USB_CONF_H

#include "stm32f4xx.h"
#include <sys/cdefs.h>

#define USE_DEVICE_MODE

#define USB_OTG_FS_CORE
#define USE_USB_OTG_FS

#define RX_FIFO_FS_SIZE                          128
#define TX0_FIFO_FS_SIZE                          64
#define TX1_FIFO_FS_SIZE                         128
#define TX2_FIFO_FS_SIZE                          0
#define TX3_FIFO_FS_SIZE                          0

#define __ALIGN_BEGIN
#define __ALIGN_END __attribute__ ((aligned (4)))

#endif //VMG_USB_CONF_H
