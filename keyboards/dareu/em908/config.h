/* Copyright 2023 HorrorTroll <https://github.com/HorrorTroll>
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

//#define POINTING_DEVICE_SCLK_PIN B3
//#define POINTING_DEVICE_SDIO_PIN B4
#define POINTING_DEVICE_CS_PIN B2

#define SPI_DRIVER SPID0
#define SPI_SCK_PIN B3
#define SPI_SCK_FLAGS PAL_MODE_INPUT_ANALOG
#define SPI_MOSI_PIN B5
#define SPI_MOSI_FLAGS PAL_MODE_INPUT_ANALOG
#define SPI_MISO_PIN B4
#define SPI_MISO_FLAGS PAL_MODE_INPUT_ANALOG
