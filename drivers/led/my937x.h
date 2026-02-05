// Copyright (c) 2024 Hansem Ro <hansemro@outlook.com>
// Copyright 2025 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * LED or RGB Matrix driver for MY-SEMI MY937X LED drivers with proprietary
 * SPI-like control interface using PWM for GCK and General Purpose Timer
 * (GPT) for flushing color data.
 *
 * This driver can support the following MY-SEMI families:
 * - MY9373
 * - MY9374
 *
 * MY937X drivers only require the Frame Start, Command Data
 * and Scan instructions.
 *
 * This driver does not pre-define configuration settings.
 */

#include <stdint.h>
#include "gpio.h"

/* Setting command data of MY937X(s) */
#ifndef MY937X_COMMAND_DATA
#    define MY937X_COMMAND_DATA 0x0000803F // 0b00000000000000001000000000111111 (Default)
#endif

/* Number of output channels of a single MY937X */
#ifndef MY937X_NUM_CHANNELS
#    error "MY937X_NUM_CHANNELS is not defined"
#endif

/* Number of MY937X(s) */
#ifndef MY937X_NUM_DRIVER
#    error "MY937X_NUM_DRIVER is not defined"
#endif

/* LED matrix directions:
 * - COL2ROW: from MCU-managed column pins to MY937X-managed row pins
 * - ROW2COL: from MCU-managed row pins to MY937X-managed column pins
 */
#define COL2ROW 0
#define ROW2COL 1

#ifndef MY937X_LED_DIRECTION
#    error "MY937X_LED_DIRECTION is not defined to either ROW2COL or COL2ROW"
#endif

/* MCU-managed row/column LED pins */
#ifndef MY937X_LED_GPIO_PINS
#    error "MY937X_LED_GPIO_PINS is not defined with an array of pins"
#endif

#ifndef MY937X_LED_GPIO_OUTPUT_MODE
#    define MY937X_LED_GPIO_OUTPUT_MODE PAL_MODE_OUTPUT_PUSHPULL
#endif

#ifndef MY937X_NUM_LED_GPIO_PINS
#    error "MY937X_NUM_LED_GPIO_PINS is not defined"
#else
#    define MY937X_LED_COUNT (MY937X_NUM_CHANNELS * MY937X_NUM_LED_GPIO_PINS)
#endif

#define ACTIVE_LOW 0
#define ACTIVE_HIGH 1

#ifdef MY937X_LED_GPIO_ACTIVE_STATE
#    if (MY937X_LED_GPIO_ACTIVE_STATE != ACTIVE_HIGH) && (MY937X_LED_GPIO_ACTIVE_STATE != ACTIVE_LOW)
#        error "MY937X_LED_GPIO_ACTIVE_STATE must be defined to either ACTIVE_HIGH or ACTIVE_LOW"
#    endif
#else
#    error "MY937X_LED_GPIO_ACTIVE_STATE is not defined"
#endif

/* PWM driver to use for generating GCK clock signal */
#ifndef MY937X_PWM_DRIVER
#    error "MY937X_PWM_DRIVER is not defined"
#endif

#ifndef MY937X_PWM_CHANNEL
#    error "MY937X_PWM_CHANNEL is not defined"
#endif

#ifndef MY937X_PWM_OUTPUT_MODE
#    define MY937X_PWM_OUTPUT_MODE PWM_OUTPUT_ACTIVE_LOW
#endif

/* PWM period in counter ticks >= 2 */
#ifndef MY937X_PWM_PERIOD
#    define MY937X_PWM_PERIOD 2UL
#endif

/* PWM counter frequency in Hz = desired GCK frequency * MY937X_PWM_PERIOD */
#ifndef MY937X_PWM_COUNTER_FREQUENCY
/* Default: 4 MHz GCK */
#    define MY937X_PWM_COUNTER_FREQUENCY (4000000UL * MY937X_PWM_PERIOD)
#endif

/* GPT driver to use for continuous row/column pin cycling and data flushing */
#ifndef MY937X_GPT_DRIVER
#    error "MY937X_GPT_DRIVER is not defined"
#endif

/* Timer period in counter ticks >= 2 */
#ifndef MY937X_GPT_PERIOD
#    define MY937X_GPT_PERIOD 2UL
#endif

/* Timer counter frequency in Hz = desired LED refresh rate * MY937X_NUM_LED_GPIO_PINS * MY937X_GPT_PERIOD */
#ifndef MY937X_GPT_COUNTER_FREQUENCY
/* Default: 120 Hz LED refresh rate */
#    define MY937X_GPT_COUNTER_FREQUENCY (120UL * MY937X_NUM_LED_GPIO_PINS * MY937X_GPT_PERIOD)
#endif

/* MY937X pins config */
#ifndef MY937X_LAT_PIN
#    error "MY937X_LAT_PIN is not defined"
#endif

#ifndef MY937X_LAT_OUTPUT_MODE
#    define MY937X_LAT_OUTPUT_MODE PAL_MODE_OUTPUT_PUSHPULL
#endif

#ifndef MY937X_SDI_PIN
#    error "MY937X_SDI_PIN is not defined"
#endif

#ifndef MY937X_SDI_OUTPUT_MODE
#    define MY937X_SDI_OUTPUT_MODE PAL_MODE_OUTPUT_PUSHPULL
#endif

#ifndef MY937X_DCK_PIN
#    error "MY937X_DCK_PIN is not defined"
#endif

#ifndef MY937X_DCK_OUTPUT_MODE
#    define MY937X_DCK_OUTPUT_MODE PAL_MODE_OUTPUT_PUSHPULL
#endif

#ifndef MY937X_GCK_PIN
#    error "MY937X_GCK_PIN is not defined"
#endif

#ifndef MY937X_GCK_OUTPUT_MODE
#    define MY937X_GCK_OUTPUT_MODE PAL_MODE_OUTPUT_PUSHPULL
#endif

/* (Optional) MCU-managed pin to enable power to MY937X */
#ifdef MY937X_POWER_ENABLE_PIN
#    if !defined(MY937X_POWER_ACTIVE_STATE) && (MY937X_POWER_ACTIVE_STATE != ACTIVE_HIGH) && (MY937X_POWER_ACTIVE_STATE != ACTIVE_LOW)
#        error "MY937X_POWER_ACTIVE_STATE must be defined to either ACTIVE_HIGH or ACTIVE_LOW"
#    endif
#    ifndef MY937X_POWER_OUTPUT_MODE
#        define MY937X_POWER_OUTPUT_MODE PAL_MODE_OUTPUT_PUSHPULL
#    endif
#endif

/* Insert nop delays to operate under 20MHz */
#ifndef MY937X_NOPS
#    if defined(__AVR__)
#        define MY937X_NOPS 0 // AVR at 16 MHz already spends 62.5 ns per clock, so no extra delay is needed
#    elif defined(PROTOCOL_CHIBIOS)
#        include "hal.h"
#        include "chibios_config.h"
#        if defined(STM32F0XX) || defined(STM32F1XX) || defined(STM32F3XX) || defined(STM32F4XX) || defined(STM32L0XX) || defined(AT32F415) || defined(GD32VF103) || defined(MCU_RP)
#            define MY937X_NOPS (60 / (1000000000L / (CPU_CLOCK / 3))) // This calculates how many loops of 3 nops to run to delay 60 ns
#        else
#            error MY937X_NOPS configuration required
#            define MY937X_NOPS 0 // this just pleases the compile so the above error is easier to spot
#        endif
#    endif
#endif

#define my937x_io_wait                          \
    do {                                        \
        for (int i = 0; i < MY937X_NOPS; i++) { \
            __asm__ volatile("nop\n\t"          \
                             "nop\n\t"          \
                             "nop\n\t");        \
        }                                       \
    } while (0)

/* Send 'instr' number of DCK pulses while LAT is asserted high */
void my937x_dck_pulses(uint8_t instr);

/* 5 DCK pulses for frame start */
void my937x_frame_start(void);

/* Send SDI bit when DCK pulses for command data and scans */
void my937x_sdi_bit(uint8_t bit);

/* Send 32-bit DCK pulses for command data */
void my937x_send_32bits(uint32_t word);

/* Send 16-bit DCK pulses for scans */
void my937x_send_16bits(uint16_t word);

/* Write 32-bit command data to each driver (32-bit × M), then single LAT pulse */
void my937x_command_data(void);

/* Write 16-bit scans to 16 OUT and each driver (16-bit × 16 OUT x M),
   then single LAT pulse */
void my937x_scan(uint16_t scan_line[MY937X_NUM_DRIVER][MY937X_NUM_CHANNELS]);

/* Initialize MY937X driver(s) */
void my937x_init_drivers(void);
void my937x_init_pins(void);
void my937x_init_command_data(void);
void my937x_init_timers(void);

/* Write RGB color to back buffer at a specific index */
void my937x_set_color(int index, uint8_t red, uint8_t green, uint8_t blue);
/* Write RGB color to entire back buffer */
void my937x_set_color_all(uint8_t red, uint8_t green, uint8_t blue);
/* Updates front buffer from back buffer */
void my937x_flush(void);

enum my937x_color_ch {
    MY937X_UNUSED_CH,
    MY937X_RED_CH,
    MY937X_GREEN_CH,
    MY937X_BLUE_CH,
};

typedef struct {
    uint8_t color_channel;
    uint8_t color_index; // MY937X ROW/COL index for the color channel
} my937x_channel_t;

/* Output channel to color channel mapping */
extern const my937x_channel_t g_my937x_channels[MY937X_NUM_DRIVER][MY937X_NUM_CHANNELS];

/* LED Matrix to LED Index */
#if (MY937X_LED_DIRECTION == ROW2COL)
extern const uint8_t g_my937x_led_matrix_co[MY937X_NUM_LED_GPIO_PINS][MY937X_NUM_CHANNELS];
#elif (MY937X_LED_DIRECTION == COL2ROW)
extern const uint8_t g_my937x_led_matrix_co[MY937X_NUM_CHANNELS][MY937X_NUM_LED_GPIO_PINS];
#endif

/* LED row/column pins */
extern pin_t g_my937x_led_pins[MY937X_NUM_LED_GPIO_PINS];
