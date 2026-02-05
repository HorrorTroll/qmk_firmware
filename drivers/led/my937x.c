// Copyright (c) 2024 Hansem Ro <hansemro@outlook.com>
// Copyright 2025 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <string.h>
#include "gpio.h"
#include "progmem.h"
#include "my937x.h"

#ifdef SPLIT_KEYBOARD
#    error "Split keyboard not supported"
#endif

pin_t g_my937x_led_pins[MY937X_NUM_LED_GPIO_PINS] = MY937X_LED_GPIO_PINS;

static uint8_t led_gpio_idx = 0;
static uint16_t scan_data[MY937X_NUM_LED_GPIO_PINS][MY937X_NUM_DRIVER][MY937X_NUM_CHANNELS];

// TODO: validate PWM output frequency if user-defined

/* GCK frequency = MY937X_PWM_COUNTER_FREQUENCY / MY937X_PWM_PERIOD */
static PWMConfig gck_pwm_config = {
    .frequency = MY937X_PWM_COUNTER_FREQUENCY,
    .period    = MY937X_PWM_PERIOD,
};

typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
} my937x_led_t;

// my937x_leds[0]: back buffer
// my937x_leds[1]: front buffer (sent to MY937X)
static my937x_led_t my937x_leds[2][MY937X_LED_COUNT];

const uint16_t CIE1931_16_CURVE[256] PROGMEM = {
        0,    28,    57,    85,   114,   142,   171,   199,   228,   256,   285,   313,   341,   370,   398,   427,
      455,   484,   512,   541,   569,   598,   627,   658,   689,   721,   755,   789,   825,   861,   899,   937,
      977,  1018,  1060,  1103,  1147,  1192,  1239,  1287,  1336,  1386,  1437,  1490,  1544,  1599,  1656,  1714,
     1773,  1834,  1896,  1959,  2024,  2090,  2157,  2226,  2297,  2369,  2442,  2517,  2593,  2671,  2751,  2832,
     2914,  2999,  3085,  3172,  3261,  3352,  3444,  3538,  3634,  3732,  3831,  3932,  4035,  4139,  4245,  4354,
     4464,  4575,  4689,  4804,  4922,  5041,  5162,  5285,  5410,  5537,  5666,  5797,  5930,  6065,  6202,  6341,
     6482,  6626,  6771,  6918,  7068,  7220,  7373,  7529,  7687,  7848,  8010,  8175,  8342,  8512,  8683,  8857,
     9033,  9212,  9393,  9576,  9762,  9949, 10140, 10333, 10528, 10725, 10926, 11128, 11333, 11541, 11751, 11963,
    12179, 12396, 12617, 12840, 13065, 13293, 13524, 13757, 13993, 14232, 14474, 14718, 14965, 15215, 15467, 15722,
    15980, 16241, 16505, 16771, 17041, 17313, 17588, 17866, 18147, 18431, 18717, 19007, 19300, 19596, 19894, 20196,
    20501, 20809, 21119, 21433, 21750, 22071, 22394, 22720, 23050, 23383, 23719, 24058, 24400, 24746, 25095, 25447,
    25802, 26161, 26523, 26888, 27257, 27629, 28004, 28383, 28765, 29151, 29540, 29932, 30328, 30728, 31131, 31537,
    31947, 32360, 32777, 33198, 33622, 34050, 34481, 34916, 35355, 35797, 36243, 36693, 37146, 37603, 38064, 38529,
    38997, 39469, 39945, 40425, 40908, 41396, 41887, 42382, 42881, 43384, 43891, 44401, 44916, 45435, 45957, 46484,
    47015, 47549, 48088, 48631, 49178, 49728, 50283, 50843, 51406, 51973, 52545, 53120, 53700, 54284, 54873, 55465,
    56062, 56663, 57269, 57878, 58492, 59111, 59733, 60360, 60992, 61627, 62268, 62912, 63561, 64215, 64873, 65535
};

static void my937x_gpt_flush_isr(GPTDriver *gptp) {
    /* Blank GCK frame insertion (0% PWM duty cycle) */
    pwmDisableChannel(&MY937X_PWM_DRIVER, MY937X_PWM_CHANNEL - 1);

    /* Disable ROW/COL pins */
    for (int i = 0; i < MY937X_NUM_LED_GPIO_PINS; i++) {
#if (MY937X_LED_GPIO_ACTIVE_STATE == ACTIVE_LOW)
        gpio_write_pin_high(g_my937x_led_pins[i]);
#else
        gpio_write_pin_low(g_my937x_led_pins[i]);
#endif
    }

    /* Send frame start */
    my937x_frame_start();

    /* Send command data */
    my937x_command_data();

    /* Enable ROW/COL pins */
#if (MY937X_LED_GPIO_ACTIVE_STATE == ACTIVE_LOW)
    gpio_write_pin_low(g_my937x_led_pins[led_gpio_idx]);
#else
    gpio_write_pin_high(g_my937x_led_pins[led_gpio_idx]);
#endif

    led_gpio_idx += 1;
    led_gpio_idx = (led_gpio_idx >= MY937X_NUM_LED_GPIO_PINS) ? 0 : led_gpio_idx;

    /* Flush data for next ROW/COL */
    for (int i = MY937X_NUM_CHANNELS - 1; i >= 0; i--) {
        uint8_t color_ch;
        uint8_t my937x_ch_idx;
        uint8_t led_idx;
        uint16_t color_val;
        for (int j = MY937X_NUM_DRIVER - 1; j >= 0; j--) {
            color_ch = g_my937x_channels[j][i].color_channel;
            my937x_ch_idx = g_my937x_channels[j][i].color_index;
#if (MY937X_LED_DIRECTION == ROW2COL)
            led_idx = g_my937x_led_matrix_co[led_gpio_idx][my937x_ch_idx];
#elif (MY937X_LED_DIRECTION == COL2ROW)
            led_idx = g_my937x_led_matrix_co[my937x_ch_idx][led_gpio_idx];
#endif
            switch (color_ch) {
                case MY937X_RED_CH:
                    color_val = pgm_read_word(&CIE1931_16_CURVE[my937x_leds[1][led_idx].r]);
                    break;
                case MY937X_GREEN_CH:
                    color_val = pgm_read_word(&CIE1931_16_CURVE[my937x_leds[1][led_idx].g]);
                    break;
                case MY937X_BLUE_CH:
                    color_val = pgm_read_word(&CIE1931_16_CURVE[my937x_leds[1][led_idx].b]);
                    break;
                case MY937X_UNUSED_CH:
                default:
                    color_val = 0;
            }

            scan_data[led_gpio_idx][j][i] = color_val;
        }
    }

    my937x_scan(scan_data[led_gpio_idx]);

    /* Start GCK frame (50% PWM duty cycle) */
    pwmEnableChannel(&MY937X_PWM_DRIVER, MY937X_PWM_CHANNEL - 1, PWM_PERCENTAGE_TO_WIDTH(&MY937X_PWM_DRIVER, 5000));

    /* Disable SDI and DCK pin */
    gpio_write_pin_low(MY937X_SDI_PIN);
    gpio_write_pin_low(MY937X_DCK_PIN);
}

/* ROW/COL update frequency = MY937X_GPT_COUNTER_FREQUENCY / MY937X_GPT_PERIOD */
static GPTConfig my937x_gpt_config = {
    .frequency = MY937X_GPT_COUNTER_FREQUENCY,
    .callback  = my937x_gpt_flush_isr,
};

/* Send 'instr' number of DCK pulses while LAT is asserted high */
void inline my937x_dck_pulses(uint8_t instr) {
    gpio_write_pin_low(MY937X_LAT_PIN);
    my937x_io_wait;
    gpio_write_pin_high(MY937X_LAT_PIN);
    while (instr-- > 0) {
        my937x_io_wait;
        gpio_write_pin_high(MY937X_DCK_PIN);
        my937x_io_wait;
        gpio_write_pin_low(MY937X_DCK_PIN);
    }
    gpio_write_pin_low(MY937X_LAT_PIN);
}

/* 5 DCK pulses for frame start */
void inline my937x_frame_start(void) {
    my937x_dck_pulses(5);
}

/* Send SDI bit when DCK pulses for command data and scans */
void inline my937x_sdi_bit(uint8_t bit) {
    gpio_write_pin(MY937X_SDI_PIN, bit);
    my937x_io_wait;
    gpio_write_pin_high(MY937X_DCK_PIN);
    my937x_io_wait;
    gpio_write_pin_low(MY937X_DCK_PIN);
}

/* Send 32-bit DCK pulses for command data */
void inline my937x_send_32bits(uint32_t word) {
    for (int i = 31; i >= 0; i--) {
        my937x_sdi_bit((word >> i) & 1);
    }
}

/* Send 16-bit DCK pulses for scans */
void inline my937x_send_16bits(uint16_t word) {
    for (int i = 15; i >= 0; i--) {
        my937x_sdi_bit((word >> i) & 1);
    }
}

/* Write 32-bit command data to each driver (32-bit × M), then single LAT pulse */
void inline my937x_command_data(void) {
    for (int i = 0; i < MY937X_NUM_DRIVER; i++) {
        my937x_send_32bits(MY937X_COMMAND_DATA);
    }

    gpio_write_pin_high(MY937X_LAT_PIN);
    my937x_io_wait;
    gpio_write_pin_low(MY937X_LAT_PIN);
    my937x_io_wait;
}

/* Write 16-bit scans to 16 OUT and each driver (16-bit × 16 OUT x M),
   then single LAT pulse */
void inline my937x_scan(uint16_t scan_line[MY937X_NUM_DRIVER][MY937X_NUM_CHANNELS]) {
    for (int j = MY937X_NUM_DRIVER - 1; j >= 0; j--) {
        for (int i = MY937X_NUM_CHANNELS - 1; i >= 0; i--) {
            my937x_send_16bits(scan_line[j][i]);
        }
    }

    gpio_write_pin_high(MY937X_LAT_PIN);
    my937x_io_wait;
    gpio_write_pin_low(MY937X_LAT_PIN);
    my937x_io_wait;
}

/* Configure and initialize MY937X and LED pins */
__attribute__((weak)) void my937x_init_pins(void) {
    palSetLineMode(MY937X_DCK_PIN, MY937X_DCK_OUTPUT_MODE);
    palSetLineMode(MY937X_GCK_PIN, MY937X_GCK_OUTPUT_MODE);
    palSetLineMode(MY937X_LAT_PIN, MY937X_LAT_OUTPUT_MODE);
    palSetLineMode(MY937X_SDI_PIN, MY937X_SDI_OUTPUT_MODE);

    /* Setup LED ROW/COL pins */
    for (int i = 0; i < MY937X_NUM_LED_GPIO_PINS; i++) {
        palSetLineMode(g_my937x_led_pins[i], MY937X_LED_GPIO_OUTPUT_MODE);
    }

    /* Enable power to MY937X if managed by MCU */
#ifdef MY937X_POWER_ENABLE_PIN
    palSetLineMode(MY937X_POWER_ENABLE_PIN, MY937X_POWER_OUTPUT_MODE);
#    if (MY937X_POWER_ACTIVE_STATE == ACTIVE_HIGH)
    gpio_write_pin_high(MY937X_POWER_ENABLE_PIN);
#    else
    gpio_write_pin_low(MY937X_POWER_ENABLE_PIN);
#    endif
#endif
}

__attribute__((weak)) void my937x_init_timers(void) {
    /* Configure PWM (for GCK pin) */
    gck_pwm_config.channels[MY937X_PWM_CHANNEL - 1].mode = MY937X_PWM_OUTPUT_MODE;
    pwmStart(&MY937X_PWM_DRIVER, &gck_pwm_config);

    /* Start PWM channel for GCK (50% PWM duty cycle) */
    pwmEnableChannel(&MY937X_PWM_DRIVER, MY937X_PWM_CHANNEL - 1, PWM_PERCENTAGE_TO_WIDTH(&MY937X_PWM_DRIVER, 5000));

    /* Configure GPT */
    gptStart(&MY937X_GPT_DRIVER, &my937x_gpt_config);
    if (MY937X_GPT_DRIVER.state == GPT_READY)
        gptStartContinuous(&MY937X_GPT_DRIVER, MY937X_GPT_PERIOD);
}

__attribute__((weak)) void my937x_init_drivers(void) {
    /* Configure pins */
    my937x_init_pins();

    /* Start/configure GCK PWM and GPT */
    my937x_init_timers();
}

void my937x_set_color(int index, uint8_t red, uint8_t green, uint8_t blue) {
    my937x_leds[0][index].r = red;
    my937x_leds[0][index].g = green;
    my937x_leds[0][index].b = blue;
}

void my937x_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
    for (int i = 0; i < MY937X_LED_COUNT; i++) {
        my937x_set_color(i, red, green, blue);
    }
}

void my937x_flush(void) {
    memcpy(&my937x_leds[1], &my937x_leds[0], MY937X_LED_COUNT * sizeof(my937x_led_t));
}
