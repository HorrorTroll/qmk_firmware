// Copyright 2021 SonixQMK
// Copyright 2026 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <hal.h>

#if !defined(FEE_PAGE_SIZE) || !defined(FEE_TOTAL_PAGES) || !defined(FEE_DENSITY_PAGES)
#    ifndef FEE_PAGE_SIZE
#        define FEE_PAGE_SIZE (uint16_t)0x0040 // Page size = 64bytes
#    endif
#    ifndef FEE_TOTAL_PAGES
#        define FEE_TOTAL_PAGES 480 // How many pages are available
#    endif
#    ifndef FEE_DENSITY_PAGES
#        define FEE_DENSITY_PAGES 23 // How many pages are used as EEPROM
#    endif
#endif

#if !defined(FEE_MCU_FLASH_SIZE)
#    define FEE_MCU_FLASH_SIZE 30 // Size in Kb
#endif

/* Start of the emulated eeprom */
#if !defined(FEE_PAGE_BASE_ADDRESS)
#    ifndef FEE_PAGE_BASE_ADDRESS
#        define FEE_PAGE_BASE_ADDRESS ((uint32_t)(FEE_PAGE_SIZE * FEE_TOTAL_PAGES - ((FEE_DENSITY_PAGES + 1) * FEE_PAGE_SIZE))) // Guard the last page
#    endif
#endif
