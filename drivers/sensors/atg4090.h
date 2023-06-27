/* Copyright 2021 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2019 Hiroyuki Okada
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

// Definitions for the ATG4090 serial line.
#ifndef ATG4090_SCLK_PIN
#    ifdef POINTING_DEVICE_SCLK_PIN
#        define ATG4090_SCLK_PIN POINTING_DEVICE_SCLK_PIN
#    else
#        error "No clock pin defined -- missing POINTING_DEVICE_SCLK_PIN or ATG4090_SCLK_PIN"
#    endif
#endif

#ifndef ATG4090_SDIO_PIN
#    ifdef POINTING_DEVICE_SDIO_PIN
#        define ATG4090_SDIO_PIN POINTING_DEVICE_SDIO_PIN
#    else
#        error "No data pin defined -- missing POINTING_DEVICE_SDIO_PIN or ATG4090_SDIO_PIN"
#    endif
#endif

#ifndef ATG4090_CS_PIN
#    ifdef POINTING_DEVICE_CS_PIN
#        define ATG4090_CS_PIN POINTING_DEVICE_CS_PIN
#    else
#        error "No chip select pin defined -- missing POINTING_DEVICE_CS_PIN or ATG4090_CS_PIN define"
#    endif
#endif

typedef struct {
    int8_t dx;
    int8_t dy;
} report_atg4090_t;

// A bunch of functions to implement the ATG4090-specific serial protocol.
// Mostly taken from ADNS5050 driver.
// Note that the "serial.h" driver is insufficient, because it does not
// manually manipulate a serial clock signal.
void             atg4090_init(void);
void             atg4090_sync(void);
uint8_t          atg4090_serial_read(void);
void             atg4090_serial_write(uint8_t data);
uint8_t          atg4090_read_reg(uint8_t reg_addr);
void             atg4090_write_reg(uint8_t reg_addr, uint8_t data);
report_atg4090_t atg4090_read_burst(void);
void             atg4090_set_cpi(uint16_t cpi);
uint16_t         atg4090_get_cpi(void);
int8_t           convert_twoscomp(uint8_t data);
bool             atg4090_check_signature(void);

#if !defined(ATG4090_CPI)
#    define ATG4090_CPI 1000
#endif

#define ATG4090_CPI_STEP 250
#define ATG4090_CPI_MIN 250
#define ATG4090_CPI_MAX 3500
