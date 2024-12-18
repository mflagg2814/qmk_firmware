/* Copyright 2023 @ Keychron (https://www.keychron.com)
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
#include "keychron_common.h"

enum layers { _BASE, _FN1 };

enum user_custom_keycodes {
	// https://getreuer.info/posts/keyboards/macros/index.html
	SRCHSEL = SAFE_RANGE,
	SELLINE,
	JOINLN,
	JIGGLE,
	SORTLINES
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_numpad_6x4(
        KC_ESC,   KC_TAB,  KC_BSPC,   MO(_FN1),
        KC_NUM,   KC_PSLS, KC_PAST,   KC_PMNS,
        KC_P7,    KC_P8,   KC_P9,
        KC_P4,    KC_P5,   KC_P6,     KC_PPLS,
        KC_P1,    KC_P2,   KC_P3,
        KC_P0,             KC_PDOT,   KC_PENT),

    [_FN1] = LAYOUT_numpad_6x4(
        KC_CALC,  KC_LPRN, KC_RPRN,   _______,
        RGB_TOG,  RGB_MOD, RGB_RMOD,  RGB_SPD,
        _______,  _______, _______,
        _______,  _______, _______,   RGB_SPI,
        SELLINE,  JOINLN,  _______,
        SORTLINES,           SRCHSEL,   JIGGLE)
};

void housekeeping_task_user(void) {
    housekeeping_task_keychron();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keychron(keycode, record)) {
        return false;
    }
	
	if (record->event.pressed) {
		static deferred_token token = INVALID_DEFERRED_TOKEN;
		static report_mouse_t report = {0};
		if (token) {
		  // If jiggler is currently running, stop when any key is pressed.
		  cancel_deferred_exec(token);
		  token = INVALID_DEFERRED_TOKEN;
		  report = (report_mouse_t){};  // Clear the mouse.
		  host_mouse_send(&report);
		} else if (keycode == JIGGLE) {
		    uint32_t jiggler_callback(uint32_t trigger_time, void* cb_arg) {
				// Deltas to move in a circle of radius 20 pixels over 32 frames.
				static const int8_t deltas[32] = {
					0, -1, -2, -2, -3, -3, -4, -4, -4, -4, -3, -3, -2, -2, -1, 0,
					0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 3, 3, 2, 2, 1, 0};
				static uint8_t phase = 0;
				// Get x delta from table and y delta by rotating a quarter cycle.
				report.x = deltas[phase];
				report.y = deltas[(phase + 8) & 31];
				phase = (phase + 1) & 31;
				host_mouse_send(&report);
				return 16;  // Call the callback every 16 ms.
			}

			token = defer_exec(1, jiggler_callback, NULL);  // Schedule callback.
		}
	}
	
	switch (keycode) {
    case SRCHSEL:  // Searches the current selection in a new tab.
		if (record->event.pressed) {
			SEND_STRING(SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER));
		}
		return false;
	case SELLINE:  // Selects the current line.
		if (record->event.pressed) {
			SEND_STRING(SS_TAP(X_HOME) SS_LSFT(SS_TAP(X_END)));
		}
		return false;
	case SORTLINES:  // Searches the current selection in a new tab.
		if (record->event.pressed) {
			SEND_STRING(SS_LCTL(SS_LALT(SS_LSFT("s"))));
		}
		return false;
	case JOINLN:  // Join lines like Vim's `J` command.
		if (record->event.pressed) {
			SEND_STRING( // Go to the end of the line and tap delete.
				SS_TAP(X_END) SS_TAP(X_DEL)
				// In case this has joined two words together, insert one space.
				SS_TAP(X_SPC)
				SS_LCTL(
				  // Go to the beginning of the next word.
				  SS_TAP(X_RGHT) SS_TAP(X_LEFT)
				  // Select back to the end of the previous word. This should select				 
				  // all spaces and tabs between the joined lines from indentation
				  // or trailing whitespace, including the space inserted earlier.
				  SS_LSFT(SS_TAP(X_LEFT) SS_TAP(X_RGHT)))
				// Replace the selection with a single space.
				SS_TAP(X_SPC));
		}
		return false;
	}
	
    return true;
}

/*
    [_BASE] = LAYOUT_numpad_6x4(
        MO(_FN1), KC_ESC,  KC_BSPC,   KC_TAB,
        KC_NUM,   KC_PSLS, KC_PAST,   KC_PMNS,
        KC_P7,    KC_P8,   KC_P9,
        KC_P4,    KC_P5,   KC_P6,     KC_PPLS,
        KC_P1,    KC_P2,   KC_P3,
        KC_P0,             KC_PDOT,   KC_PENT),

    [_FN1] = LAYOUT_numpad_6x4(
        _______,    KC_MUTE, KC_VOLD,   KC_VOLU,
        RGB_MOD,    RGB_VAI, RGB_HUI,   KC_DEL,
        RGB_RMOD,   RGB_VAD, RGB_HUD,
        RGB_SAI,    RGB_SPI, KC_MPRV,   _______,
        RGB_SAD,    RGB_SPD, KC_MPLY,
        RGB_TOG,             KC_MNXT,   _______),
		
	    [_RESERVED1] = LAYOUT_numpad_6x4(
        _______,    _______, _______,   _______,
        _______,    _______, _______,   _______,
        _______,    _______, _______,
        _______,    _______, _______,   _______,
        _______,    _______, _______,
        _______,             _______,   _______),

    [_RESERVED2] = LAYOUT_numpad_6x4(
        _______,    _______, _______,   _______,
        _______,    _______, _______,   _______,
        _______,    _______, _______,
        _______,    _______, _______,   _______,
        _______,    _______, _______,
        _______,             _______,   _______)
*/