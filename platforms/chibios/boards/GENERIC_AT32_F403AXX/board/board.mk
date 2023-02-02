# List of all the board related files.
BOARDSRC = ${CHIBIOS_CONTRIB}/os/hal/boards/GENERIC_AT32_F403AXX/board.c

# Required include directories
BOARDINC = ${CHIBIOS_CONTRIB}/os/hal/boards/GENERIC_AT32_F403AXX

# Shared variables
ALLCSRC += $(BOARDSRC)
ALLINC  += $(BOARDINC)
