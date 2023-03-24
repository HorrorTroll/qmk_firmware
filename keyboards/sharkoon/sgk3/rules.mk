# project specific files
SRC = ../../../drivers/led/sn32/matrix_sn32f240.c

# MCU name
MCU = SN32F248F

# Build Options
#   comment out to disable the options.
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no        # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output

# Custom matrix handling
CUSTOM_MATRIX = yes

# Custom RGB matrix handling
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = SN32F24x

LAYOUTS = fullsize_ansi fullsize_iso

# Build ansi by default
DEFAULT_FOLDER = sharkoon/sgk3/ansi

# ENABLE OPENRGB
OPENRGB_ENABLE = yes
