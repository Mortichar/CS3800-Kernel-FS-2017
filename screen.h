#pragma once

#include <stddef.h>
#include <stdint.h>
#include "port.h"

typedef enum {
	TEXT    =   0,
	VIDEO   =   1
}vga_mode;

#define CURSOR_UP 0x00
#define CURSOR_RIGHT 0x01
#define CURSOR_DOWN 0x02
#define CURSOR_LEFT 0x03

extern vga_mode VGA_MODE;

/* Print an NTCA to the screen */
void terminal_writestring(const char* data);
/* Write single character to screen */
void terminal_putchar(char c);
/* Backspace */
void terminal_back(void);
/* Set up the terminal display */
void terminal_initialize(void);
/* Swap from 80x25xtext to 320x200x256 video */
void text_to_video(void);
/*Swap from 320x200x256 video to 80x25xtext text */
void video_to_text(void);
/* Clear display to solid color */
void clear_display(void);