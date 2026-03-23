PROTOCOL_DIR = protocol
CHIBIOS_DIR = $(PROTOCOL_DIR)/chibios


ifneq ($(filter $(MCU_SERIES),SN32F260),)
    CHIBIOS_USB_PROTOCOL ?= legacy
    ifeq ($(strip $(CHIBIOS_USB_PROTOCOL)), legacy)
        SRC += $(CHIBIOS_DIR)/usb_legacy/usb_main.c
        SRC += $(CHIBIOS_DIR)/usb_legacy/chibios.c
        SRC += usb_descriptor.c
        SRC += $(CHIBIOS_DIR)/usb_legacy/usb_driver.c
    else ifeq ($(strip $(CHIBIOS_USB_PROTOCOL)), async)
        USB_ASYNC_ENABLE := yes
    endif
else
    USB_ASYNC_ENABLE := yes
endif

ifeq ($(strip $(USB_ASYNC_ENABLE)), yes)
    SRC += $(CHIBIOS_DIR)/usb_main.c
    SRC += $(CHIBIOS_DIR)/chibios.c
    SRC += usb_descriptor.c
    SRC += $(CHIBIOS_DIR)/usb_driver.c
    SRC += $(CHIBIOS_DIR)/usb_endpoints.c
    SRC += $(CHIBIOS_DIR)/usb_report_handling.c
endif

SRC += $(CHIBIOS_DIR)/usb_util.c
SRC += $(LIBSRC)

VPATH += $(TMK_PATH)/$(PROTOCOL_DIR)
VPATH += $(TMK_PATH)/$(CHIBIOS_DIR)
VPATH += $(TMK_PATH)/$(CHIBIOS_DIR)/lufa_utils

OPT_DEFS += -DFIXED_CONTROL_ENDPOINT_SIZE=64
OPT_DEFS += -DFIXED_NUM_CONFIGURATIONS=1
