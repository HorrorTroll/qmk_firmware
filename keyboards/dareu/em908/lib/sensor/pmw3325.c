// Copyright 2019 Sunjun Kim
// Copyright 2019 Hiroyuki Okada
// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// Copyright 2021-2024 Colin Lam (Ploopy Corporation)
// Copyright 2026 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "pmw3325.h"
#include "wait.h"
#include "gpio.h"
#include "pointing_device_internal.h"

// Convert a 16-bit twos complement binary-represented number into a
// signed 16-bit integer.
static int16_t convert_twoscomp_16(uint8_t high, uint8_t low) {
    uint16_t data = (high << 8) | low;

    if ((data & 0x8000) == 0x8000)
        return -32768 + (data & 0x7FFF);
    else
        return data;
}

void pmw3325_serial_write(uint8_t data) {
    gpio_set_pin_output(PMW3325_SDIO_PIN);

    for (int8_t b = 7; b >= 0; b--) {
        gpio_write_pin_low(PMW3325_SCLK_PIN);

        if (data & (1 << b))
            gpio_write_pin_high(PMW3325_SDIO_PIN);
        else
            gpio_write_pin_low(PMW3325_SDIO_PIN);

        wait_us(1);

        gpio_write_pin_high(PMW3325_SCLK_PIN);
    }

    wait_us(4);
}

uint8_t pmw3325_serial_read(void) {
    gpio_set_pin_input(PMW3325_SDIO_PIN);
    uint8_t data = 0;

    for (uint8_t i = 0; i < 8; ++i) {
        gpio_write_pin_low(PMW3325_SCLK_PIN);
        wait_us(1);

        data = (data << 1) | gpio_read_pin(PMW3325_SDIO_PIN);

        gpio_write_pin_high(PMW3325_SCLK_PIN);
        wait_us(1);
    }

    return data;
}

void pmw3325_write(uint8_t reg_addr, uint8_t data) {
    gpio_write_pin_low(PMW3325_CS_PIN);
    wait_us(1); // tNCS_SCLK
    pmw3325_serial_write(reg_addr | 0b10000000);
    wait_us(180); // tSRAD
    pmw3325_serial_write(data);
    wait_us(1); // tSCLK_NCS
    gpio_write_pin_high(PMW3325_CS_PIN);
    wait_us(20); // tSRR
}

uint8_t pmw3325_read(uint8_t reg_addr) {
    gpio_write_pin_low(PMW3325_CS_PIN);
    wait_us(1); // tNCS_SCLK
    pmw3325_serial_write(reg_addr);
    wait_us(180); // tSRAD
    uint8_t data = pmw3325_serial_read();
    wait_us(1); // tSCLK_NCS
    gpio_write_pin_high(PMW3325_CS_PIN);
    wait_us(20); // tSRR

    return data;
}

bool pointing_device_driver_init(void) {
    wait_ms(50);
    gpio_set_pin_output(PMW3325_SCLK_PIN);
    gpio_set_pin_output(PMW3325_SDIO_PIN);
    gpio_set_pin_output(PMW3325_CS_PIN);

    // CS must be kept high at power-up stage for at least 1ms
    gpio_write_pin_low(PMW3325_CS_PIN);
    wait_ms(10);
    gpio_write_pin_high(PMW3325_CS_PIN);

    // reboot
    pmw3325_write(0x3A, 0x5A);
    wait_ms(10);

    pmw3325_write(0x18, 0x39);

    if (!pmw3325_check_signature()) {
        return false;
    }

    // read a burst, then discard
    pmw3325_read(0x02);
    pmw3325_read(0x03);
    pmw3325_read(0x04);
    pmw3325_read(0x05);
    pmw3325_read(0x06);

    // initialize
    pmw3325_write(0x78, 0x80);
    pmw3325_write(0x79, 0x80);
    pmw3325_write(0x14, 0x80);
    pmw3325_write(0x20, 0x40);
    pmw3325_write(0x1A, 0x40);
    pmw3325_write(0x47, 0x00);
    pmw3325_write(0x48, 0x01);
    pmw3325_write(0x60, 0x01);
    pmw3325_write(0x69, 0x03);
    pmw3325_write(0x1D, 0x90);
    pmw3325_write(0x1B, 0x2E);
    pmw3325_write(0x24, 0x05);
    pmw3325_write(0x56, 0x00);
    pmw3325_write(0x2C, 0x8A);
    pmw3325_write(0x2D, 0x58);
    pmw3325_write(0x40, 0x80);
    pmw3325_write(0x7F, 0x01);
    pmw3325_write(0x7A, 0x32);
    pmw3325_write(0x6A, 0x93);
    pmw3325_write(0x6B, 0x68);
    pmw3325_write(0x6C, 0x71);
    pmw3325_write(0x6D, 0x50);
    pmw3325_write(0x7F, 0x00);
    pmw3325_write(0x7F, 0x02);
    pmw3325_write(0x29, 0x1C);
    pmw3325_write(0x2A, 0x1A);
    pmw3325_write(0x2B, 0x90);
    pmw3325_write(0x40, 0x80);
    pmw3325_write(0x7F, 0x00);
    return true;
}

report_pmw3325_t pmw3325_read_burst(void) {
    report_pmw3325_t report = {0};

    uint8_t motion = pmw3325_read(0x02);
    if ((motion & 0b10000000) == 0b10000000) {
        // Motion detected
        uint8_t dx_l = pmw3325_read(0x03);
        uint8_t dx_h = pmw3325_read(0x04);
        uint8_t dy_l = pmw3325_read(0x05);
        uint8_t dy_h = pmw3325_read(0x06);

        report.dx = convert_twoscomp_16(dx_h, dx_l);
        report.dy = convert_twoscomp_16(dy_h, dy_l);
    }

    return report;
}

static const uint8_t pmw3325_cpi_lut[50] = {
    0x02, 0x04, 0x06, 0x08, 0x0B, 0x0D, 0x0F, 0x12, 0x14, 0x16,
    0x19, 0x1B, 0x1D, 0x20, 0x22, 0x24, 0x27, 0x29, 0x2B, 0x2E,
    0x30, 0x32, 0x34, 0x37, 0x39, 0x3B, 0x3E, 0x40, 0x42, 0x45,
    0x47, 0x49, 0x4C, 0x4E, 0x50, 0x53, 0x55, 0x57, 0x5A, 0x5C,
    0x5E, 0x61, 0x63, 0x65, 0x68, 0x6A, 0x6C, 0x6F, 0x71, 0x73
};

void pointing_device_driver_set_cpi(uint16_t cpi) {
    uint8_t cpival = CONSTRAIN((cpi / PMW3325_CPI_STEP), (PMW3325_CPI_MIN / PMW3325_CPI_STEP), (PMW3325_CPI_MAX / PMW3325_CPI_STEP)) - 1U;

    pmw3325_write(0x1B, pmw3325_cpi_lut[cpival]);
}

uint16_t pointing_device_driver_get_cpi(void) {
    uint8_t cpival = pmw3325_read(0x1B);

    for (uint8_t cpi = 0; cpi < 50; cpi++) {
        if (pmw3325_cpi_lut[cpi] == cpival)
            return (cpi + 1) * PMW3325_CPI_STEP;
    }

    return 0;
}

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    report_pmw3325_t data = pmw3325_read_burst();

    if (data.dx != 0 || data.dy != 0) {
        pd_dprintf("Raw ] X: %d, Y: %d\n", data.dx, data.dy);
        mouse_report.x = CONSTRAIN_HID_XY(data.dx);
        mouse_report.y = CONSTRAIN_HID_XY(data.dy);
    }

    return mouse_report;
}

bool pmw3325_check_signature(void) {
    uint8_t checkval_1 = pmw3325_read(0x00);
    uint8_t checkval_2 = pmw3325_read(0x3F);

    return (checkval_1 == 0x43 && checkval_2 == 0xBC);
}
