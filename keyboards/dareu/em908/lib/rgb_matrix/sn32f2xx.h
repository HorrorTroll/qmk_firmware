// Copyright 2024 Dimitris Mantzouranis <d3xter93@gmail.com>
// Copyright 2026 fightforlife
// Copyright 2026 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>

#if !defined(SN32F2XX_LED_OUTPUT_LUMINOSITY_R)
#    define SN32F2XX_LED_OUTPUT_LUMINOSITY_R 1
#endif
#if !defined(SN32F2XX_LED_OUTPUT_LUMINOSITY_G)
#    define SN32F2XX_LED_OUTPUT_LUMINOSITY_G 1
#endif
#if !defined(SN32F2XX_LED_OUTPUT_LUMINOSITY_B)
#    define SN32F2XX_LED_OUTPUT_LUMINOSITY_B 1
#endif

#if defined(RGB_MATRIX_SN32F2XX)
#    define SN32F2XX_LED_COUNT RGB_MATRIX_LED_COUNT
#endif

#if !defined(SN32F2XX_PWM_OUTPUT_ACTIVE_LEVEL)
#    define SN32F2XX_PWM_OUTPUT_ACTIVE_LEVEL SN32F2XX_PWM_OUTPUT_ACTIVE_LOW
#endif

#if !defined(SN32F2XX_RGB_OUTPUT_ACTIVE_LEVEL)
#    define SN32F2XX_RGB_OUTPUT_ACTIVE_LEVEL SN32F2XX_RGB_OUTPUT_ACTIVE_HIGH
#endif

#if !defined(SN32F2XX_PWM_CONTROL)
#    define SN32F2XX_PWM_CONTROL HARDWARE_PWM
#endif

#if !defined(SN32F2XX_PWM_DIRECTION)
#    define SN32F2XX_PWM_DIRECTION DIODE_DIRECTION
#endif

#if !defined(SN32F2XX_RGB_MATRIX_ROW_CHANNELS)
#    define SN32F2XX_RGB_MATRIX_ROW_CHANNELS 3
#endif

#if !defined(SN32F2XX_RGB_MATRIX_ROWS)
#    define SN32F2XX_RGB_MATRIX_ROWS MATRIX_ROWS
#endif

#if !defined(SN32F2XX_RGB_MATRIX_COLS)
#    define SN32F2XX_RGB_MATRIX_COLS MATRIX_COLS
#endif

#if !defined(SN32F2XX_RGB_MATRIX_ROW_PINS)
#    error Missing definition for SN32F2XX_RGB_MATRIX_ROW_PINS
#endif

#if !defined(SN32F2XX_RGB_MATRIX_COL_PINS)
#    define SHARED_MATRIX
#    define SN32F2XX_RGB_MATRIX_COL_PINS MATRIX_COL_PINS
#endif

#if !defined(SN32F2XX_RGB_MATRIX_ROWS_HW)
#    define SN32F2XX_RGB_MATRIX_ROWS_HW (SN32F2XX_RGB_MATRIX_ROWS * SN32F2XX_RGB_MATRIX_ROW_CHANNELS)
#endif

#if !defined(SN32F2XX_RGB_MATRIX_COLOR_DEPTH)
#    define SN32F2XX_RGB_MATRIX_COLOR_DEPTH 24
#endif

#if !defined(SN32F2XX_RGB_MATRIX_BUFFER)
#    define SN32F2XX_RGB_MATRIX_BUFFER TRUE
#endif

void sn32f2xx_init(void);
void sn32f2xx_flush(void);
void sn32f2xx_set_color(int index, uint8_t r, uint8_t g, uint8_t b);
void sn32f2xx_set_color_all(uint8_t r, uint8_t g, uint8_t b);
void sn32f2xx_save_color(uint8_t ledNumber, char colorChannel, uint8_t value);
uint8_t sn32f2xx_load_color(uint8_t ledNumber, char colorChannel);
void sn32f2xx_save_color_buf(uint8_t ledNumber, char colorChannel, uint8_t value);
uint8_t sn32f2xx_load_color_buf(uint8_t ledNumber, char colorChannel);
