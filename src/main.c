#include "jlibc.h"

int main(void) {
  lcd_init();

  // print_int(7);
  // print_int(69);
  // print_int(420);
  // print_int(6969);
  // print_int(69420);

  // mini_printf("Hello", NULL);

  print_str("Hello world");
 
  u8 pattern = 0b10101010;
  while(1) {
    u16 i;
    PORTB = pattern;
    pattern = (pattern << 1) | (pattern >> 7);
    
    for (i = 0; i < 12000; ++i) { asm("NOP"); }
  }
}