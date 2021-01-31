/* Copyright 2019 imchipwood
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

#define _BASE     0
#define _SUB      1
#define _DBG      2


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /*
        BASE LAYER
   /-----------------------------------------------------`
   |             |    7    |    8    |    9    |  Bkspc  |
   |             |---------|---------|---------|---------|
   |             |    4    |    5    |    6    |  +      |
   |             |---------|---------|---------|---------|
   |             |    1    |    2    |    3    |  -      |
   |-------------|---------|---------|---------|---------|
   | Play/Pause  | TT(_SUB)|    0    |    .    |  =      |
   \-----------------------------------------------------'
  */
  [_BASE] = LAYOUT(
                   KC_P7,    KC_P8,  KC_P9,   KC_BSPC, 
                   KC_P4,    KC_P5,  KC_P6,   KC_KP_PLUS, 
                   KC_P1,    KC_P2,  KC_P3,   KC_KP_MINUS, 
    KC_MPLY,       TT(_SUB), KC_P0,  KC_PDOT, KC_EQL
  ),
  /*
        SUB LAYER
   /-----------------------------------------------------`
   |             |         |RGB_M_SW | RGB_M_T | Numlock |
   |             |---------|---------|---------|---------|
   |             |         | RGB_M_B | RGB_M_R | RGB_MOD |
   |             |---------|---------|---------|---------|
   |             |         | RGB_HUI | RGB_HUD | RGB RMOD|
   |-------------|---------|---------|---------|---------|
   |  MO(_DBG)   |         | RGB_VAI | RGB_VAD | RGB_TOG |
   \-----------------------------------------------------'
  */
  [_SUB] = LAYOUT(
                 _______,     RGB_M_SW,    RGB_M_T,     KC_NLCK, 
                 _______,     RGB_M_B,     RGB_M_R,     RGB_MOD, 
                 _______,     RGB_HUI,     RGB_HUD,     RGB_RMOD, 
    MO(_DBG),    _______,     RGB_VAI,     RGB_VAD,      RGB_TOG
  ),
  /*
        DEBUG LAYER
   /-----------------------------------------------------`
   |             |         |         |         |  Reset  |
   |             |---------|---------|---------|---------|
   |             |         |         |         |         |
   |             |---------|---------|---------|---------|
   |             |         |         |         |         |
   |-------------|---------|---------|---------|---------|
   |             |         |         |         |         |
   \-----------------------------------------------------'
  */
  [_DBG] = LAYOUT(                 
                 _______,     _______,     _______,      RESET, 
                 _______,     _______,     _______,      _______, 
                 _______,     _______,     _______,      _______, 
    _______,     _______,     _______,     _______,      _______
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // If console is enabled, it will print the matrix position and status of each key pressed
// #ifdef CONSOLE_ENABLE
//     uprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);
// #endif
  return true;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  //debug_enable = true;
  //debug_matrix = true;
  //debug_keyboard = true;
  //debug_mouse = true;
}

void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

void led_set_user(uint8_t usb_led) {

}


void encoder_update_user(uint8_t index, bool clockwise) {
  /*  Custom encoder control - handles CW/CCW turning of encoder
   *  Cusotom behavior:
   */
  // left encoder
  if (index == 0) {
    switch (biton32(layer_state)) {
      case _BASE:
        // main layer : PgDN and PgUP
        if (clockwise) {
          writePin(LAYER_INDICATOR_LED_1, true);	
          wait_ms(50);
          writePin(LAYER_INDICATOR_LED_1, false);	
          //tap_code(KC_PGUP);
          tap_code(KC_MS_WH_UP);
        } else {
          writePin(LAYER_INDICATOR_LED_0, true);	
          wait_ms(50);
          writePin(LAYER_INDICATOR_LED_0, false);
          //tap_code(KC_PGDN);
          tap_code(KC_MS_WH_DOWN);
        }
        break;

      case _SUB:
        // sub layer : Vol + /- 
        // built-in LED is reverse, false is on, true is off
        if (clockwise) {
          writePin(LAYER_INDICATOR_LED_2, false);	
          wait_ms(50);
          writePin(LAYER_INDICATOR_LED_2, true);	
          tap_code(KC_VOLU);
        } else {
          writePin(LAYER_INDICATOR_LED_3, false);	
          wait_ms(50);
          writePin(LAYER_INDICATOR_LED_3, true);
          tap_code(KC_VOLD);
        }
        break;

      default:
        // any other layer (shouldn't exist..) - volume up (CW) and down (CCW)
        if (clockwise) {
          tap_code(KC_MS_WH_UP);
        } else {
          tap_code(KC_MS_WH_DOWN);
        }
        break;   
    }
  } 
  /* disable 2nd encoder
  else if (index == 1) {
    // right encoder
    switch (biton32(layer_state)) {
      case _BASE:
        // main layer - volume up (CW) and down (CCW)
        if (clockwise) {
          writePin(LAYER_INDICATOR_LED_0, true);	
          wait_ms(50);
          writePin(LAYER_INDICATOR_LED_0, false);	
          tap_code(KC_VOLU);
        } else {
          tap_code(KC_VOLD);
        }
        break;

      case _SUB:
        // sub layer - next track (CW) and previous track (CCW)
        if (clockwise) {
          writePin(LAYER_INDICATOR_LED_0, true);	
	        wait_ms(50);
	        writePin(LAYER_INDICATOR_LED_0, false);	
          tap_code(KC_WH_U);
        } else {	
          tap_code(KC_WH_D);
        }
        break;

      default:
        // any other layer (shouldn't exist..) - volume up (CW) and down (CCW)
        if (clockwise) {
          tap_code(KC_VOLU);
        } else {
          tap_code(KC_VOLD);
        }
        break;   
    }
  }
 */ 
}
