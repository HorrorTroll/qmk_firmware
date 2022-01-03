/* Copyright 2021 HorrorTroll <https://github.com/HorrorTroll>
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

/* RGB matrix size */
#define LED_MATRIX_COLS 15
#define LED_MATRIX_ROWS 5
#define LED_MATRIX_ROW_CHANNELS 3
#define LED_MATRIX_ROWS_HW (LED_MATRIX_ROWS * LED_MATRIX_ROW_CHANNELS)

/* RGB matrix pin               0    1,   2,   3,   4,   5,   6,   7,  8,  9, 10, 11, 12,  13   14 */
#define LED_MATRIX_COL_PINS {  A8,  A9, A10, A11, A12, A13, A14, A15, B0, B1, B2, B3, B4,  B5,  B6 }
#define LED_MATRIX_ROW_PINS { B13, B14, B15, C15,  C0,  C1,  C3,  C4, C5, C6, C7, C8, C9, C10, C11 }

/* RGB led number */
#define DRIVER_LED_TOTAL (68)
