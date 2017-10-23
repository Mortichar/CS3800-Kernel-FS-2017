#include "interrupt_handlers.h"

__attribute__((interrupt))
void keypress(void* frame)
{
  char buf[3];
  getScancodeByte(buf);
  terminal_writestring(buf);
  PIC_sendEOI(0x1);
}

__attribute__((interrupt))
void hi(void* frame)
{
  terminal_writestring("hi!\n");
}
