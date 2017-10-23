#include "ps2.h"

char nybble2char(uint8_t nybble)
{
  nybble &= 0x0F; // Clear top bits
  if(nybble >= 10)
    return nybble + 'A' - 10;
  return nybble + '0';
}

void byte2chars(uint8_t byte, char* buf)
{
  buf[0] = nybble2char(byte >> 4);
  buf[1] = nybble2char(byte);
  buf[2] = '\0';
}

uint8_t getScancode_loop()
{
    uint8_t c=0;
    static uint8_t last=0;
    do {
        c = inb(0x60);
        if(c != last) {
            last = c;
            return c;
        }
    } while(1);
}

uint8_t getScancode()
{
  return inb(0x60);
}

void getScancodeByte_loop(char* buf)
{
  byte2chars(getScancode_loop(), buf);
}

void getScancodeByte(char* buf)
{
  byte2chars(getScancode(), buf);
}
