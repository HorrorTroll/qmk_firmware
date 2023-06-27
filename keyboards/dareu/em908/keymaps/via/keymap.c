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

#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum layer_names {
    _BASE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
       ┌───┐         ┌───┐
       │   │  ┌───┐  │   │
       │LMB│  │WHL│  │RMB│
       │   │  └───┘  │   │
       └───┘         └───┘
       ┌───┐  ┌───┐
	   │SB1│  │DPI│
       ├───┤  └───┘
       │SB2│
       └───┘
*/
    /*  Row:    0        1        2        3        4        5      */
    [_BASE] = LAYOUT(
                KC_BTN1, KC_BTN2, KC_BTN3, KC_BTN4, KC_NO,   KC_BTN5
            )
};
