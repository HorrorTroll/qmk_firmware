// Copyright 2019 Sunjun Kim
// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// Copyright 2020 Ploopy Corporation
// Copyright 2026 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "em908.h"
#include "quantum.h"

#ifdef POINTING_DEVICE_ENABLE
// for legacy support
#ifndef MOUSE_DPI_OPTIONS
#    define MOUSE_DPI_OPTIONS \
        { 400, 800, 1200, 1600, 3200, 5000 }
#    ifndef MOUSE_DPI_DEFAULT
#        define MOUSE_DPI_DEFAULT 1
#    endif
#endif
#ifndef MOUSE_DPI_DEFAULT
#    define MOUSE_DPI_DEFAULT 0
#endif

keyboard_config_t keyboard_config;
uint16_t          dpi_array[] = MOUSE_DPI_OPTIONS;
#define DPI_OPTION_SIZE ARRAY_SIZE(dpi_array)

void cycle_dpi(void) {
    keyboard_config.dpi_config = (keyboard_config.dpi_config + 1) % DPI_OPTION_SIZE;
    eeconfig_update_kb(keyboard_config.raw);
    pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
}

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    mouse_report = pointing_device_task_user(mouse_report);

    return mouse_report;
}

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    if (!process_record_user(keycode, record)) {
        return false;
    }

    if (keycode == DPI_CFG && record->event.pressed) {
        cycle_dpi();
    }

    return true;
}

// Hardware Setup
void pointing_device_init_kb(void) {
    keyboard_config.raw = eeconfig_read_kb();
    if (keyboard_config.dpi_config > DPI_OPTION_SIZE) {
        eeconfig_init_kb();
    }
    pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
}

void eeconfig_init_kb(void) {
    keyboard_config.dpi_config = MOUSE_DPI_DEFAULT;
    eeconfig_update_kb(keyboard_config.raw);
    eeconfig_init_user();
}
#endif

#ifdef RGB_MATRIX_ENABLE
#    include "lib/rgb_matrix/sn32f2xx.h"

static void rgb_matrix_driver_init(void) {
    sn32f2xx_init();
}

static void rgb_matrix_driver_flush(void) {
    sn32f2xx_flush();
}

static void rgb_matrix_driver_set_color(int index, uint8_t r, uint8_t g, uint8_t b) {
    sn32f2xx_set_color(index, r, g, b);
}

static void rgb_matrix_driver_set_color_all(uint8_t r, uint8_t g, uint8_t b) {
    sn32f2xx_set_color_all(r, g, b);
}

// clang-format off
const rgb_matrix_driver_t rgb_matrix_driver = {
    .init          = rgb_matrix_driver_init,
    .flush         = rgb_matrix_driver_flush,
    .set_color     = rgb_matrix_driver_set_color,
    .set_color_all = rgb_matrix_driver_set_color_all,
};
// clang-format on
#endif
