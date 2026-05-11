#include "jlibc.h"

u16 monitor_addr;

__attribute__((noinline)) void scroll_up(void) {
  monitor_addr += 8;
}

__attribute__((noinline)) void scroll_down(void) {
  monitor_addr -= 8;
}

int main(void) {
  lcd_init();
  monitor_addr = 0x8000;

  // draw initial state
  lcd_clear();
  print_page(monitor_addr);
  lcd_send_command(LCD_LINE2);
  print_page(monitor_addr + 4);

  while (1) {
    u8 buttons = PORTA & 0b00000011;
    
    if (buttons & 0b00000001) {
      scroll_up();
      lcd_clear();
      
      print_page(monitor_addr);
      lcd_send_command(LCD_LINE2);
      print_page(monitor_addr + 4);
      // wait for release
      while (PORTA & 0b00000001);
    } else if (buttons & 0b00000010) {
      scroll_down();
      lcd_clear();

      print_page(monitor_addr);
      lcd_send_command(LCD_LINE2);
      print_page(monitor_addr + 4);
      // wait for release
      while (PORTA & 0b00000010);
    }
  }
}