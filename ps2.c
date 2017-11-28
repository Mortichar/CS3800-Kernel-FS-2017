#include "ps2.h"

const unsigned char set1codes[128] =
{
  0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 0, 0, 0, 0,
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0, 'a','s',
  'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
  'b', 'n', 'm', ',', '.', '/', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const unsigned char set1codes_shift[128] = {
  0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', 0, 0, 0, 0,
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0, 0, 'A','S',
  'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V',
  'B', 'N', 'M', '<', '>', '?', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


uint8_t lshift_held = 0;
uint8_t rshift_held = 0;

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
  //why are scan codes converted to ascii?
  byte2chars(getScancode(), buf);
}
