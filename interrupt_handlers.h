#pragma once
#include "screen.h"
#include "ps2.h"
#include "interrupt.h"

/* Print the scancode for a keyboard event to the screen */
void keypress(void* frame);

/* Print 'hi' to the screen */
void hi(void* frame);
