#include "interrupt_handlers.h"

void handle_keypress(uint8_t c);

__attribute__((interrupt))
void keypress(void* frame)
{
  uint8_t c = getScancode();//top
  PIC_sendEOI(0x1);
  handle_keypress(c);//bottom
}

__attribute__((interrupt))
void hi(void* frame)
{
  terminal_writestring("hi!\n");
}

void handle_keypress(uint8_t c){
  if(c == 0x8F){
    if(VGA_MODE == TEXT){
      text_to_video();
      clear_display();
      VGA_MODE = VIDEO;
      return;
    }else{
      video_to_text();
      VGA_MODE = TEXT;
      return;
    } //In video mode; TODO: More video mode options
  }
  if(VGA_MODE == TEXT){
      if(c == KEY_LSHIFT_DOWN){//capitalization
    lshift_held = 1;
  }else if(c == KEY_LSHIFT_UP){
    lshift_held = 0;
  }

  if(c == KEY_RSHIFT_DOWN){
    rshift_held = 1;
  }else if(c == KEY_RSHIFT_UP){
    rshift_held = 0;
  }
  if(c > END_OF_DOWN){//Key release
    return;
  }
  if(c == KEY_SPACE_DOWN){//Spacebar
    terminal_putchar(0);
    return;
  }
  if(c == KEY_BACKSPACE_DOWN){
    terminal_back();
    return;
  }
  if(c == KEY_RETURN_DOWN){
    terminal_putchar('\n');
    return;
  }
    //Convert to ASCII
    c = set1codes[c];
    if(!c){//character not handled
      return;
    }
    if(lshift_held || rshift_held){
      if(c > 0x0B){
        terminal_putchar((c - 32) % 128);
      } // no capitalization on numbers
    }else{
        terminal_putchar(c);
    }
  }
  if(VGA_MODE == VIDEO){
  }
}
