cmake_minimum_required(VERSION 3.13)
project(ledblink C ASM)

include(../../toolchain.cmake)
include(../f405.cmake)

add_executable(ledblink.axf
        main.c
        ../common/delay.c
        startup_stm32f40_41xxx.s
        system_stm32f4xx.c
        ${STDPERIPH_SRC}/misc.c
        ${STDPERIPH_SRC}/stm32f4xx_gpio.c
        ${STDPERIPH_SRC}/stm32f4xx_tim.c
        ${STDPERIPH_SRC}/stm32f4xx_rcc.c)

target_include_directories(ledblink.axf PUBLIC
        .
        ${CMSIS_INC}
        ${DEVICE_INC}
        ${STDPERIPH_INC})
