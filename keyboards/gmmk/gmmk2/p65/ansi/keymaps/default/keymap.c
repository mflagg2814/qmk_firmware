/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the imp`lied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#define _QWERTY         0
#define _FN_FOR_QWERTY  1
#define _DVORAK         2
#define _FN_FOR_DVORAK  3

static uint32_t CAPS_LOCK_FLASH_TIMER = 0;
static bool IN_GAME_MODE = false;

enum custom_keycodes {
	GAME_MODE = SAFE_RANGE,
	F5_KEY,
    MUTE_MIC,
	TASK_MGR,
	WIN_KEY,
	CAPS_LOCK,
	HDR_TOGGLE
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
	case GAME_MODE:
        if (record->event.pressed) {
            IN_GAME_MODE = !IN_GAME_MODE;
        }
        break;
	case F5_KEY:
		if (IN_GAME_MODE) {
			break;
		}
	
        if (record->event.pressed) {
            register_code(KC_F5);
        } else {
            unregister_code(KC_F5);
        }
        break;
    case MUTE_MIC:
		if (IN_GAME_MODE) {
			break;
		}
	
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_LSFT);
            register_code(KC_M);
        } else {
            unregister_code(KC_LCTL);
            unregister_code(KC_LSFT);
            unregister_code(KC_M);
        }
        break;
	case TASK_MGR:
		if (IN_GAME_MODE) {
			break;
		}
	
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_LSFT);
            register_code(KC_ESC);
        } else {
            unregister_code(KC_LCTL);
            unregister_code(KC_LSFT);
            unregister_code(KC_ESC);
        }
        break;
	case WIN_KEY:
		if (IN_GAME_MODE) {
			break;
		}
	
        if (record->event.pressed) {
            register_code(KC_LGUI);
        } else {
            unregister_code(KC_LGUI);
        }
        break;
	case CAPS_LOCK:
	{
        if (record->event.pressed) {
            register_code(KC_CAPS);
			CAPS_LOCK_FLASH_TIMER = timer_read32(); // reset timer
        } else {
            unregister_code(KC_CAPS);
        }
        break;
	}
	case HDR_TOGGLE:
        if (record->event.pressed) {
            register_code(KC_LGUI);
            register_code(KC_LALT);
            register_code(KC_B);
        } else {
            unregister_code(KC_LGUI);
            unregister_code(KC_LALT);
            unregister_code(KC_B);
        }
        break;
    }

    return true;
};

// Fancy callback that allows overriding current RGB effect
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	const uint8_t caps_key_idx = 30;
	
	const uint8_t game_mode_key_idx = 43;
	const uint8_t task_mgr_key_idx = 57;
	const uint8_t win_key_idx = 59;
	const uint8_t mute_mic_key_idx = 63;
	
	const uint8_t dvorak_toggle_key_idx = 14;
	
	// Flash the caps lock key if enabled
    if (host_keyboard_led_state().caps_lock) {
		// time in ms
		if (timer_elapsed32(CAPS_LOCK_FLASH_TIMER) < 750) {
			RGB_MATRIX_INDICATOR_SET_COLOR(caps_key_idx, 0xFF, 0xFF, 0xFF);
		} else if (timer_elapsed32(CAPS_LOCK_FLASH_TIMER) < 1500) {
			RGB_MATRIX_INDICATOR_SET_COLOR(caps_key_idx, 0x80, 0x80, 0x80);
		} else {
			CAPS_LOCK_FLASH_TIMER = timer_read32(); // reset timer
		}
    }
	
	// Turn game mode toggle key and affected keys white if enabled
	if (IN_GAME_MODE) {
		RGB_MATRIX_INDICATOR_SET_COLOR(game_mode_key_idx, 0xFF, 0xFF, 0xFF);
		RGB_MATRIX_INDICATOR_SET_COLOR(task_mgr_key_idx, 0xFF, 0xFF, 0xFF);
		RGB_MATRIX_INDICATOR_SET_COLOR(win_key_idx, 0xFF, 0xFF, 0xFF);
		RGB_MATRIX_INDICATOR_SET_COLOR(mute_mic_key_idx, 0xFF, 0xFF, 0xFF);
	}
	
	// Turn Dvorak toggle key white if enabled
	const uint32_t current_layer = biton32(layer_state);	
	if (current_layer == _DVORAK || current_layer == _FN_FOR_DVORAK) {
		RGB_MATRIX_INDICATOR_SET_COLOR(dvorak_toggle_key_idx, 0xFF, 0xFF, 0xFF);
	}
}

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
// Default layer
[_QWERTY] = LAYOUT(
  KC_GESC,       KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,        KC_9,       KC_0,     KC_MINS,     KC_EQL,    KC_BSPC,      KC_INS,
   KC_TAB,       KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       KC_Y,       KC_U,       KC_I,        KC_O,       KC_P,     KC_LBRC,    KC_RBRC,    KC_BSLS,      KC_DEL,
CAPS_LOCK,       KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       KC_H,       KC_J,       KC_K,        KC_L,    KC_SCLN,     KC_QUOT,     KC_ENT,                  F5_KEY,
  KC_LSPO,       KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       KC_N,       KC_M,    KC_COMM,      KC_DOT,    KC_SLSH,     KC_RSPC,      KC_UP,                TASK_MGR,
  KC_LCTL,    WIN_KEY,      MO(1), /*switch to layer 1 while held*/        KC_SPC,                             KC_RALT,   MUTE_MIC,     KC_LEFT,    KC_DOWN,    KC_RGHT),
  
[_FN_FOR_QWERTY] = LAYOUT(
   KC_GRV,      KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,       KC_F9,     KC_F10,      KC_F11,     KC_F12,    _______,       TO(2), /*switch to layer 2*/
  _______,    _______,    RGB_VAI,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    KC_PSCR,     _______,    KC_PAUS,    _______,     _______,
  _______,    RGB_RMOD,   RGB_VAD,    RGB_MOD,    _______,    _______,    _______,    _______,    _______,     _______,    _______,     _______,    _______,               GAME_MODE,
  _______,    RGB_HUI,    RGB_HUD,    RGB_SPD,    RGB_SPI,    RGB_SAD,    RGB_SAI,    KC_MUTE,    KC_MPRV,     KC_MPLY,    KC_MNXT,     _______,    KC_PGUP,              HDR_TOGGLE,
  _______,    _______,    _______,                                          RESET,                             _______,    _______,     KC_HOME,    KC_PGDN,     KC_END),

[_DVORAK] = LAYOUT(
  KC_GESC,       KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,       KC_7,       KC_8,        KC_9,       KC_0,     KC_LBRC,    KC_RBRC,    KC_BSPC,      KC_INS,
   KC_TAB,    KC_QUOT,    KC_COMM,     KC_DOT,       KC_P,       KC_Y,       KC_F,       KC_G,       KC_C,        KC_R,       KC_L,     KC_SLSH,     KC_EQL,    KC_BSLS,      KC_DEL,
CAPS_LOCK,       KC_A,       KC_O,       KC_E,       KC_U,       KC_I,       KC_D,       KC_H,       KC_T,        KC_N,       KC_S,     KC_MINS,     KC_ENT,                  F5_KEY,
  KC_LSPO,    KC_SCLN,       KC_Q,       KC_J,       KC_K,       KC_X,       KC_B,       KC_M,       KC_W,        KC_V,       KC_Z,     KC_RSPC,      KC_UP,                TASK_MGR,
  KC_LCTL,    WIN_KEY,      MO(3), /*switch to layer 3 while held*/        KC_SPC,                             KC_RALT,   MUTE_MIC,     KC_LEFT,    KC_DOWN,    KC_RGHT),

[_FN_FOR_DVORAK] = LAYOUT(
   KC_GRV,      KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,       KC_F9,     KC_F10,      KC_F11,     KC_F12,    _______,       TO(0), /*switch to layer 0*/
  _______,    _______,    RGB_VAI,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    KC_PSCR,     _______,    KC_PAUS,    _______,     _______,
  _______,    RGB_RMOD,   RGB_VAD,    RGB_MOD,    _______,    _______,    _______,    _______,    _______,     _______,    _______,     _______,    _______,               GAME_MODE,
  _______,    RGB_HUI,    RGB_HUD,    RGB_SPD,    RGB_SPI,    RGB_SAD,    RGB_SAI,    KC_MUTE,    KC_MPRV,     KC_MPLY,    KC_MNXT,     _______,    KC_PGUP,              HDR_TOGGLE,
  _______,    _______,    _______,                                          RESET,                             _______,    _______,     KC_HOME,    KC_PGDN,     KC_END)
};
  
// Original layouts
/*
[0] = LAYOUT(
  KC_GESC,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
  KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_PGUP,
  KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,             KC_PGDN,
  KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_END,
  KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 MUTE_MIC, MO(1),    KC_LEFT,  KC_DOWN,  KC_RGHT),

[1] = LAYOUT(
   KC_GRV,    KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,   KC_F10,   KC_F11,   KC_F12,   _______,  KC_INS,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PSCR,  KC_SCRL,  KC_PAUS,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,             _______,
  _______,  _______,  RGB_HUI,  RGB_HUD,  RGB_SPD,  RGB_SPI,  KC_MUTE,  KC_VOLU,  KC_VOLD,  KC_MPRV,  KC_MPLY,  KC_MNXT,  _______,   RGB_VAI,  KC_HOME,
  _______,  _______,  _______,                                RESET,                                  _______,  _______,  RGB_RMOD,  RGB_VAD,  RGB_MOD),

[2] = LAYOUT(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,             _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,   _______,  _______)
};
*/

