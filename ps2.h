#pragma once
#include <stdint.h>
#include "port.h"

/* Gets the current scancode from the keyboard
 * and converts it into 2 hex digits
 */

//TODO: 2 byte keypress definitions and implementations
// (ex: rctrl)

#define KEY_SPACE_DOWN 0x39
#define KEY_TAB_DOWN 0x0F
#define KEY_BACKSPACE_DOWN 0x0E
#define KEY_LSHIFT_DOWN 0x2A
#define KEY_RSHIFT_DOWN 0x36
#define KEY_LCTRL_DOWN 0x1D
#define KEY_RETURN_DOWN 0x1C
#define KEY_UP_DOWN 0xE0, 0x48
#define KEY_RIGHT_DOWN 0xE0, 0x4D
#define KEY_DOWN_DOWN 0xE0, 0x50
#define KEY_LEFT_DOWN 0xE0, 0x4B

#define KEY_SPACE_UP 0xB9
#define KEY_TAB_UP 0x8F
#define KEY_BACKSPACE_UP 0x8E
#define KEY_LSHIFT_UP 0xAA
#define KEY_RSHIFT_UP 0xB6
#define KEY_LCTRL_UP 0x9D
#define KEY_RETURN_UP 0x9C
#define KEY_UP_UP 0xE0, 0xC8
#define KEY_RIGHT_UP 0xE0, 0xCD 
#define KEY_DOWN_UP 0xE0, 0xD0
#define KEY_LEFT_UP 0xE0, 0xCB

#define END_OF_DOWN 0x80

/*PS2 Set 1 Scancodes
wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1
*/

/*
 * This is a 128 entry array with ASCII values for all of the scancodes.
 * set1codes[scancode_value] will give you the ASCII for that scancode
 * Note: Special functions, such as the escape key, may be undefined,
 *  along with many KEY_xxxxx_UPs
 */
extern const unsigned char set1codes[];
extern const unsigned char set1codes_shift[];
extern uint8_t lshift_held;
extern uint8_t rshift_held;

void getScancodeByte(char* buf);

/* Get the current scancode from the keyboard */
uint8_t getScancode();

/* Loops until the keyboard scancode updates,
 * then converts the scancode into 2 hex digits
 *
 * You should probably not use this--the CPU just sits in a loop
 * reading the keyboard, so you can't really multitask here.
 * Use getScancodeByte() and an interrupt handler instead.
 */
void getScancodeByte_loop(char* buf);

/* Loops until the keyboard scancode updates,
 * then returns the new scancode.
 */
uint8_t getScancode_loop();

/* Given a byte, represent it as 2 hex digits */
void byte2chars(uint8_t byte, char* buf);