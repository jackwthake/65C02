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

void lcd_send_command(unsigned char cmd) {
  PORTB = cmd;

  PORTA = 0;
  PORTA = LCD_E;
  NOP5000();
  PORTA = 0;
}

void put_c(char c) {
  PORTB = c;

  PORTA = LCD_RS;
  PORTA = LCD_RS | LCD_E;
  NOP5000();
  PORTA = LCD_RS;
}


// TODO: Fix this - static buf probably causes some issue with BSS / data init
void print_int(int x) {
  static char buf[6]; // max 5 digits for 16-bit int + null
  static u8 i;
  static u8 len;
  
  if (x < 0) {
    put_c('-');
    x = -x;
  }
  
  if (x < 10) {
    put_c('0' + (u8)x);
    return;
  }
  
  // build digits in reverse
  i = 0;
  while (x > 0) {
    buf[i] = '0' + (x % 10);
    x /= 10;
    i++;
  }
  
  // print in correct order
  len = i;
  while (i > 0) {
    i--;
    put_c(buf[i]);
  }
}

void print_str(const char *str) {
  char c;
  unsigned i = 0;
  
  if (str == NULL) { 
    return;
  }
  
  while ((c = *(str + i)) != '\0') {
    put_c(c);

    ++i;
  }
}


// TODO: fix this. i dont even know lmao
__attribute__((noinline)) 
void mini_printf(char *fmt, void **argv) {
  u8 i = 0;
  u8 len = 0;
  u8 arg_idx = 0;
  u8 j = 0;
  char c = 0;
  char *str = NULL;

  i = 0;
  arg_idx = 0;
  len = (u8)strnlen(fmt, 32);

  while (i < len) {
    if ((unsigned char)fmt[i] != '%') {
      // inlined put_c
      PORTB = fmt[i];
      PORTA = LCD_RS;
      PORTA = LCD_RS | LCD_E;
      NOP5000();
      PORTA = LCD_RS;
    } else {
      i++;
      if ((unsigned char)fmt[i] == 's' && argv[arg_idx] != NULL) {
        // inlined print_str
        str = (char *)argv[arg_idx];
        j = 0;
        while ((c = str[j]) != '\0') {
          PORTB = c;
          PORTA = LCD_RS;
          PORTA = LCD_RS | LCD_E;
          NOP5000();
          PORTA = LCD_RS;
          j++;
        }
        arg_idx++;
      }
    }
    i++;
  }
}