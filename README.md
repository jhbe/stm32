# Summary

Collection of projects for STM32 microprocessors.

# Projects

- matekf405/ledblink: Firmware for a Matek F405STD board that flashes the red and blue LEDs.
- matek405/usbtest/usbtest-device: Firmware for a Matek F405STD board implementing a basic bulk
USB interface using the [lib32_usb](https://github.com/dmitrystu/libusb_stm32.git) drivers.
- matek405/usbtest/usbtest-host: The linux application for the usbtest-device.

# Setup

Download the arm tool chain from [here](https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2)
and extract it to the stm32 root.

Download the STM32 F4 Standard Periphiral Libraries from [here](https://www.st.com/en/embedded-software/stsw-stm32065.html)
and extract it to the stm32 root.

With the stm32 repo as the current directory:

    git clone https://github.com/dmitrystu/libusb_stm32.git
    cd libusb_stm32
    git clone https://github.com/ARM-software/CMSIS_5.git CMSIS
    git clone --recurse-submodules https://github.com/dmitrystu/stm32h.git CMSIS/Device/ST


Your stm32 directory should look like this now:

    gcc-arm-none-eabi-9-2020-q2-update
    libusb_stm32
    matekf405
    README.md
    STM32F4xx_DSP_StdPeriph_Lib_V1.8.0

# References

- [F405 Reference manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405415-stm32f407417-stm32f427437-and-stm32f429439-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
