#pragma once

#include <stddef.h>
#include <stdint.h>

/* Print an NTCA to the screen */
void terminal_writestring(const char* data);

/* Set up the terminal display */
void terminal_initialize(void);
