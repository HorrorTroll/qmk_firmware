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

#include "atg4090.h"
#include "wait.h"
#include "debug.h"
#include "gpio.h"

void atg4090_init(void) {
    // Initialize sensor serial pins.
    setPinOutput(ATG4090_SCLK_PIN);
    setPinOutput(ATG4090_SDIO_PIN);
    setPinOutput(ATG4090_CS_PIN);

    wait_ms(100);

    // reboot the sensor.
    atg4090_write_reg(0x3a, 0x5a);
    // unknown register 0x18.
    atg4090_write_reg(0x18, 0x39);

    // wait maximum time before sensor is ready.
    // this ensures that the sensor is actually ready after reset.
    wait_ms(20);

    // read a burst from the sensor and then discard it.
    // gets the sensor ready for write commands
    // (for example, setting the dpi).
    atg4090_read_burst();

    wait_ms(20);

    // unknown register 0x78.
    atg4090_write_reg(0x78, 0x80);
    // unknown register 0x79.
    atg4090_write_reg(0x79, 0x80);
    // unknown register 0x14.
    atg4090_write_reg(0x14, 0x80);
    // unknown register 0x20.
    atg4090_write_reg(0x20, 0x40);
    // unknown register 0x1a.
    atg4090_write_reg(0x1a, 0x40);
    // unknown register 0x47.
    atg4090_write_reg(0x47, 0x00);
    // unknown register 0x48.
    atg4090_write_reg(0x48, 0x01);
    // unknown register 0x60.
    atg4090_write_reg(0x60, 0x01);
    // unknown register 0x69.
    atg4090_write_reg(0x69, 0x03);
    // unknown register 0x1d.
    atg4090_write_reg(0x1d, 0x90);
    // unknown register 0x1b.
    atg4090_write_reg(0x1b, 0x2e);
    // unknown register 0x24.
    atg4090_write_reg(0x24, 0x05);
    // unknown register 0x56.
    atg4090_write_reg(0x56, 0x00);
    // unknown register 0x2c.
    atg4090_write_reg(0x2c, 0x8a);
    // unknown register 0x2d.
    atg4090_write_reg(0x2d, 0x58);
    // unknown register 0x40.
    atg4090_write_reg(0x40, 0x80);
    // unknown register 0x7f.
    atg4090_write_reg(0x7f, 0x01);
    // unknown register 0x7a.
    atg4090_write_reg(0x7a, 0x32);
    // unknown register 0x6a.
    atg4090_write_reg(0x6a, 0x93);
    // unknown register 0x6b.
    atg4090_write_reg(0x6b, 0x68);
    // unknown register 0x6c.
    atg4090_write_reg(0x6c, 0x71);
    // unknown register 0x6d.
    atg4090_write_reg(0x6d, 0x50);
    // unknown register 0x7f.
    atg4090_write_reg(0x7f, 0x00);
    // unknown register 0x7f.
    atg4090_write_reg(0x7f, 0x02);
    // unknown register 0x29.
    atg4090_write_reg(0x29, 0x1c);
    // unknown register 0x2a.
    atg4090_write_reg(0x2a, 0x1a);
    // unknown register 0x2b.
    atg4090_write_reg(0x2b, 0x90);
    // unknown register 0x40.
    atg4090_write_reg(0x40, 0x80);
    // unknown register 0x7f.
    atg4090_write_reg(0x7f, 0x00);
}

// Perform a synchronization with sensor.
// Just as with the serial protocol, this is used by the slave to send a
// synchronization signal to the master.
void atg4090_sync(void) {
    writePinLow(ATG4090_CS_PIN);
    wait_us(1);
    writePinHigh(ATG4090_CS_PIN);
}

void atg4090_cs_select(void) {
    writePinLow(ATG4090_CS_PIN);
}

void atg4090_cs_deselect(void) {
    writePinHigh(ATG4090_CS_PIN);
}

uint8_t atg4090_serial_read(void) {
    setPinInput(ATG4090_SDIO_PIN);
    uint8_t byte = 0;

    for (uint8_t i = 0; i < 8; ++i) {
        writePinLow(ATG4090_SCLK_PIN);
        wait_us(1);

        byte = (byte << 1) | readPin(ATG4090_SDIO_PIN);

        writePinHigh(ATG4090_SCLK_PIN);
        wait_us(1);
    }

    return byte;
}

void atg4090_serial_write(uint8_t data) {
    setPinOutput(ATG4090_SDIO_PIN);

    for (int8_t b = 7; b >= 0; b--) {
        writePinLow(ATG4090_SCLK_PIN);

        if (data & (1 << b))
            writePinHigh(ATG4090_SDIO_PIN);
        else
            writePinLow(ATG4090_SDIO_PIN);

        wait_us(2);

        writePinHigh(ATG4090_SCLK_PIN);
    }

    // This was taken from ADNS5050 driver.
    // There's no any info in ATG4090 datasheet about this...
    // tSWR. See page 15 of the ADNS5050 spec sheet.
    // Technically, this is only necessary if the next operation is an SDIO
    // read. This is not guaranteed to be the case, but we're being lazy.
    wait_us(4);

    // Note that tSWW is never necessary. All write operations require at
    // least 32us, which exceeds tSWW, so there's never a need to wait for it.
}

// Read a byte of data from a register on the sensor.
uint8_t atg4090_read_reg(uint8_t reg_addr) {
    atg4090_cs_select();

    atg4090_serial_write(reg_addr);

    uint8_t byte = atg4090_serial_read();

    // This was taken directly from ADNS5050 driver...
    // tSRW & tSRR. See page 15 of the ADNS5050 spec sheet.
    // Technically, this is only necessary if the next operation is an SDIO
    // read or write. This is not guaranteed to be the case.
    // Honestly, this wait could probably be removed.
    wait_us(1);

    atg4090_cs_deselect();

    return byte;
}

void atg4090_write_reg(uint8_t reg_addr, uint8_t data) {
    atg4090_cs_select();
    atg4090_serial_write(0b10000000 | reg_addr);
    atg4090_serial_write(data);
    atg4090_cs_deselect();
}

report_atg4090_t atg4090_read_burst(void) {
    atg4090_cs_select();

    report_atg4090_t data;
    data.dx = 0;
    data.dy = 0;

    atg4090_serial_write(0x16);

    uint8_t x = atg4090_serial_read();
    uint8_t y = atg4090_serial_read();

    // Probably burst mode may include contents of delta_xy register,
    // which contain HI parts of x/y deltas, but I had no luck finding it.
    // Probably it's required to activate 12-bit mode to access this data.
    // So we end burst mode early to not read unneeded information.
    atg4090_cs_deselect();

    data.dx = convert_twoscomp(x);
    data.dy = convert_twoscomp(y);

    return data;
}

// Convert a two's complement byte from an unsigned data type into a signed
// data type.
int8_t convert_twoscomp(uint8_t data) {
    if ((data & 0x80) == 0x80)
        return -128 + (data & 0x7F);
    else
        return data;
}

uint16_t atg4090_get_cpi(void) {
    uint8_t cpival = atg4090_read_reg(0x1b);
    // 0x1F is an inversion of 0x20 which is 0b100000
    return (uint16_t)((cpival & 0x1F) * ATG4090_CPI_STEP);
}

void atg4090_set_cpi(uint16_t cpi) {
    uint8_t cpival = constrain((cpi / ATG4090_CPI_STEP) - 1U, 0, (ATG4090_CPI_MAX / ATG4090_CPI_STEP) - 1U);
    // Fifth bit is probably a control bit.
    // ATG4090 datasheet don't have any info on this, so this is a pure guess.
    atg4090_write_reg(0x1b, 0x2e | cpival);
}

bool atg4090_check_signature(void) {
    uint8_t pid  = atg4090_read_reg(0x00);
    uint8_t pid2 = atg4090_read_reg(0x3f);

    return (pid == 0x43 && pid2 == 0xbc);
}
