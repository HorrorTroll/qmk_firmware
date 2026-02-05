// Copyright 2025 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Wear-leveling SPI EEPROM Driver
#define SPI_DRIVER SPID2
#define SPI_SCK_PIN B13
#define SPI_MOSI_PIN B15
#define SPI_MISO_PIN B14

#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN B12
#define EXTERNAL_FLASH_CLOCK_DIVISOR 4

// RGB LED Matrix Driver
// MY937X configuration
#define MY937X_NUM_CHANNELS 16
#define MY937X_NUM_DRIVER 3
#define MY937X_PWM_DRIVER PWMD1
#define MY937X_PWM_CHANNEL 1
#define MY937X_PWM_OUTPUT_MODE PWM_OUTPUT_ACTIVE_HIGH
#define MY937X_GPT_DRIVER GPTD3

// MCU manages row pins; MY937X's manage column pins
#define MY937X_LED_DIRECTION ROW2COL
// MCU-managed LED row pins (active-high)
#define MY937X_LED_GPIO_PINS \
    { C8, C7, C6, B10, F5 }
#define MY937X_NUM_LED_GPIO_PINS 5
#define MY937X_LED_GPIO_ACTIVE_STATE ACTIVE_HIGH

// MCU-managed MY937X pins
#define MY937X_LAT_PIN A4
#define MY937X_DCK_PIN A5
#define MY937X_SDI_PIN A7
#define MY937X_GCK_PIN A8
#define MY937X_GCK_OUTPUT_MODE PAL_MODE_ALTERNATE_PUSHPULL

// MCU-managed MY937X 3.3V power supply enable (active-high)
#define MY937X_POWER_ENABLE_PIN B2
#define MY937X_POWER_ACTIVE_STATE ACTIVE_HIGH

#define DEBUG_MATRIX_SCAN_RATE
