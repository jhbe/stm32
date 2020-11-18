cmake_minimum_required(VERSION 3.13)
project(usbtest-device C CXX ASM)

set(CMAKE_CXX_STANDARD 14)

include(../../../toolchain.cmake)
include(../../f405.cmake)

add_executable(usbtest-device.axf
        usbtest.c
        ../../common/delay.c
        startup_stm32f40_41xxx.s
        system_stm32f4xx.c
        ${LIB_USB_SRC}/usbd_core.c
        ${LIB_USB_SRC}/usbd_stm32f429_otgfs.c
        ${STDPERIPH_SRC}/misc.c
        ${STDPERIPH_SRC}/stm32f4xx_gpio.c
        ${STDPERIPH_SRC}/stm32f4xx_rcc.c
        ${STDPERIPH_SRC}/stm32f4xx_spi.c
        ${STDPERIPH_SRC}/stm32f4xx_tim.c)

target_include_directories(usbtest-device.axf PUBLIC
.
${DEVICE_INC}
${STDPERIPH_INC}
${USB_CORE_INC}
${LIB_USB_INC}
${LIB_CORE_INC}
${LIB_DEV_INC})
