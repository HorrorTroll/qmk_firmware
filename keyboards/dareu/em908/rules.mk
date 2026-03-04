SRC += lib/core/extra.c
SRC += lib/core/no_idle_thread.c
SRC += lib/core/rand.c
SRC += lib/eeprom/eeprom_sn32_legacy.c
SRC += lib/flash/sn32_legacy_flash_ops.c
SRC += lib/rgb_matrix/sn32f2xx.c
SRC += lib/sensor/pmw3325.c

POINTING_DEVICE_DRIVER = custom

# Reduce stack size
USE_EXCEPTIONS_STACKSIZE = 0x130
USE_PROCESS_STACKSIZE = 0x1D0

# Size and optimization flags: LTO, optimize for size, strip unused
EXTRAFLAGS += -ffunction-sections -fdata-sections -DNO_DEBUG -DNO_PRINT -fstack-usage
LDFLAGS += -Wl,--gc-sections,-Map,output.map

TOP_SYMBOLS = 10000
