/*
Copyright 2022 Dimitris Mantzouranis

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum layer_names {
    _BASE,
    _FN,
    _FN2
};

enum layer_keycodes {
    APP_GUI = SAFE_RANGE
};

enum combo_events {
    SWAP_APP_GUI,
    COMBO_LENGTH
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
       ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
       │Esc│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Bakspc│
       ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
       │ Tab │ q │ w │ e │ r │ t │ y │ u │ i │ o │ p │ [ │ ] │  \  │
       ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤
       │ Caps │ a │ s │ d │ f │ g │ h │ j │ k │ l │ ; │ ' │  Enter │
       ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤
       │ LShift │ z │ x │ c │ v │ b │ n │ m │ , │ . │ / │   RShift │
       ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
       │LCrl│GUI │LAlt│         Space          │RAlt│GUI │ Fn │RCrl│
       └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
       ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
       │ ~ │ ! │ @ │ # │ $ │ % │ ^ │ & │ * │ ( │ ) │ _ │ + │       │
       ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
       │     │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ { │ } │  |  │
       ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤
       │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ : │ " │        │
       ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤
       │ LShift │ Z │ X │ C │ V │ B │ N │ M │ < │ > │ ? │   RShift │
       ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
       │    │    │    │                        │    │    │    │    │
       └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
*/
    /*  Row:    0        1        2        3        4        5        6        7        8        9        10       11       12       13     */
    [_BASE] = LAYOUT_60_ansi(
                KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
                KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
                KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,
                KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT,
                KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(_FN), APP_GUI, KC_RCTL
            ),

/*
       ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
       │ ` │F1 │F2 │F3 │F4 │F5 │F6 │F7 │F8 │F9 │F10│F11│F12│ Delete│
       ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
       │Boot │   │ ↑ │   │   │   │   │   │Ins│   │PSc│   │   │     │
       ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤
       │      │ ← │ ↓ │ → │   │   │   │   │   │   │   │   │        │
       ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤
       │        │   │   │Cal│   │   │   │Mut│VoD│VoU│   │          │
       ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
       │    │    │    │                        │    │    │ Fn │    │
       └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
*/
    /*  Row:    0        1        2        3        4        5        6        7        8        9        10        11       12       13     */
    [_FN] = LAYOUT_60_ansi(
                KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,   KC_F11,  KC_F12,  KC_DEL,
                QK_BOOT, _______, KC_UP,   _______, _______, _______, _______, _______, KC_INS,  _______, KC_PSCR,  _______, _______, _______,
                _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, _______, _______, _______, _______,  _______,          _______,
                _______,          _______, _______, KC_CALC, _______, _______, _______, KC_MUTE, KC_VOLD, KC_VOLU,  _______,          _______,
                _______, GU_TOGG, _______,                            _______,                            MO(_FN2), _______, _______, _______
            ),

/*
       ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐
       │   │   │   │   │   │   │   │   │   │   │   │   │   │RGB_Tog│
       ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤
       │     │   │Vai│   │   │   │   │   │   │Sad│Sai│Spd│Spi│     │
       ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤
       │      │Hud│Vad│Hui│   │   │   │   │   │   │   │   │RGB_Mode│
       ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤
       │        │   │   │   │   │   │   │   │   │   │   │          │
       ├────┬───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤
       │    │    │    │                        │ Fn2│    │    │    │
       └────┴────┴────┴────────────────────────┴────┴────┴────┴────┘
*/
    /*  Row:    0        1        2        3        4        5        6        7        8        9        10       11       12       13     */
    [_FN2] = LAYOUT_60_ansi(
                _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_TOG,
                _______, _______, RGB_VAI, _______, _______, _______, _______, _______, _______, RGB_SAD, RGB_SAI, RGB_SPD, RGB_SPI, _______,
                _______, RGB_HUD, RGB_VAD, RGB_HUI, _______, _______, _______, _______, _______, _______, _______, _______,          RGB_MOD,
                _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
                _______, _______, _______,                            _______,                            _______, _______, _______, _______
            )
};

uint16_t COMBO_LEN = COMBO_LENGTH;
const uint16_t PROGMEM swap_app_gui[] = {MO(_FN), APP_GUI, KC_R, COMBO_END};

combo_t key_combos[] = {
    [SWAP_APP_GUI] = COMBO_ACTION(swap_app_gui),
};

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    switch (index) {
        case SWAP_APP_GUI:
            return 5000;
        default:
            return 50;
    }
}

bool get_combo_must_press_in_order(uint16_t index, combo_t *combo) {
    switch (index) {
        case SWAP_APP_GUI:
            return true;
        default:
            return false;

    }
}

static bool swapped = false;
void process_combo_event(uint16_t combo_index, bool pressed) {
    switch(combo_index) {
        case SWAP_APP_GUI:
            if (pressed) {
                swapped = !swapped;
            }
        break;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case APP_GUI:
            if (swapped) {
                return KC_APP;
                rgb_matrix_set_color(60, 255, 255, 255);
            } else {
                return KC_RGUI;
            }

            return false;
    }

    return true;
}
