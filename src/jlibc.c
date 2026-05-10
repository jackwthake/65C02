#include "jlibc.h"

void __attribute__((interrupt)) irq(void) {
  *(volatile unsigned char*)0x600D = 0x20;
}

unsigned strnlen(char *str, size_t max) {
  char *s = str;
  size_t len = 0;
  if (!s) {
    return 0;
  }

  while (*s != '\0' && len < max) {
    ++len;
    ++s;
  }

  return len;
}

void lcd_init(void) {
  PORTA = 0;
  DDRB = 0xFF;
  DDRA = 0b11100000;

  lcd_send_command(LCD_INIT_8BIT); // Set 8-bit mode; 2-line display; 5x8 font
  lcd_send_command(LCD_DISPL_ON); // Display on; cursor on; blink on
  lcd_send_command(LCD_DISPL_SHIFT_NORM); // Increment and shift cursor; don't shift display
}

__attribute__((noinline)) void lcd_send_command(unsigned char cmd) {
  PORTB = cmd;
  PORTA = 0;
  PORTA = LCD_E;
  NOP5000();
  PORTA = 0;
  NOP5000(); // execution time after E falls
}


void lcd_clear(void) {
  lcd_send_command(LCD_CLEAR);
  NOP5000();
  NOP5000(); // ~2.56ms, enough for the 1.52ms clear execution time
}