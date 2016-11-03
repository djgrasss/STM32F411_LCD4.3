import os

#Initialize the environment
env = DefaultEnvironment(ENV = os.environ, tools = ['as', 'gcc', 'gnulink'])

env['AR'] = 'arm-none-eabi-ar'
env['AS'] = 'arm-none-eabi-as'
env['CC'] = 'arm-none-eabi-gcc'
env['CXX'] = 'arm-none-eabi-g++'
env['LINK'] = 'arm-none-eabi-gcc'
env['RANLIB'] = 'arm-none-eabi-ranlib'
env['OBJCOPY'] = 'arm-none-eabi-objcopy'
env['PROGSUFFIX'] = '.elf'

# include locations
env['CPPPATH'] = [
    '#Inc',
    '#Drivers/CMSIS/Include',
    '#Drivers/CMSIS/Device/ST/STM32F4xx/Include',
    '#Drivers/STM32F4xx_HAL_Driver/Inc',
    '#Drivers/STM32F4xx_HAL_Driver/Inc/Legacy',
    ]

# compiler flags
env['CCFLAGS'] = [
    '-mcpu=cortex-m4',
    '-mfpu=fpv4-sp-d16',
    '-mfloat-abi=hard',
    '-mthumb',
    '-O2',
    '-fsigned-char',
    '-ffunction-sections',
    '-fdata-sections',
    '-std=gnu11',
    '-fmessage-length=0',
    '-mthumb-interwork',
    ]

# compiler flags
env['ASFLAGS'] = [
    '-mcpu=cortex-m4',
    '-mthumb',
    ]

# linker flags
env['LINKFLAGS'] = [
    '-mcpu=cortex-m4',
    '-mfpu=fpv4-sp-d16',
    '-mfloat-abi=hard',
    '-mthumb',
    '-specs=nano.specs',
    '-TSTM32F411RETx_FLASH.ld',
    '-lc',
    '-lm',
    '-lnosys',
    '-Wl,--gc-sections',
    ]

# defines
env['CPPDEFINES'] = ['STM32F411xE']

# elf = SConscript('src/SConscript', variant_dir='build', duplicate=0)

assemblyobject = env.StaticObject('Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f411xe.s')

# build everything
elf = env.Program(
    target = 'main',
    source = [
        assemblyobject,
        'Src/LCD.c',
        'Src/main.c',
        'Src/gpio.c',
        'Src/tim.c',
        'Src/stm32f4xx_hal_msp.c',
        'Src/stm32f4xx_it.c',
        'Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c',
        'Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c',
    ]
)

# binary file builder
hex=env.Command('main.hex',elf,'$OBJCOPY -O ihex -R .eeprom -R .fuse -R .lock $SOURCE $TARGET')
bin=env.Command('main.bin',elf,'$OBJCOPY -O binary -R .eeprom -R .fuse -R .lock $SOURCE $TARGET')

Default(hex, bin)

# env.Command('programbin',bin,'avrdude -p atmega32u4 -P usb -c avrispv2 -U flash:w:$SOURCE')
# env.Command('programhex',hex,'avrdude -p atmega32u4 -P usb -c avrispv2 -U flash:w:$SOURCE')
