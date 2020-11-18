SET(CMAKE_C_FLAGS "-mthumb -mcpu=cortex-m4 -D__START=main -DARMCM4 -DCORTEX_M4 -DSTM32F40_41xxx -DHSE_VALUE=8000000 -DUSE_FULL_ASSERT -DSTM32F4XX -DSTM32F405xx -DSTM32F4 -DUSE_STDPERIPH_DRIVER -ffunction-sections -fdata-sections ")
SET(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
SET(ASM_OPTIONS "${CFLAGS} -x assembler-with-cpp")
SET(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}")
SET(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs --specs=nano.specs -Wl,--gc-sections -T ../stm32_flash_f405.ld -Wl,-Map=${CMAKE_PROJECT_NAME}.map")

