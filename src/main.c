#include "jlibc.h"

void send_command(unsigned char cmd) {
  PORTB = cmd;

  PORTA = 0;
  PORTA = E;
  NOP5000();
  PORTA = 0;
}

void init_display() {
  PORTA = 0;
  DDRB = 0xFF;
  DDRA = 0b11100000;

  send_command(0b00111000); // Set 8-bit mode; 2-line display; 5x8 font
  send_command(0b00001111); // Display on; cursor on; blink on
  send_command(0b00000110); // Increment and shift cursor; don't shift display
}

int main(void) {
  init_display();

  print_str("Hello World!");

  unsigned char pattern = 0b10101010;
  while(1) {
    unsigned long int i;
    PORTB = pattern;
    pattern = (pattern << 1) | (pattern >> 7);
    
    for (i = 0; i < 12000; ++i) { asm("NOP"); }
  }
}