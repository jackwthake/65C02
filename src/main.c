#include "jlibc.h"

int main(void) {
  lcd_init();
  u16 monitor_addr = 0x8000;

  while (1) {
    lcd_clear();

    print_page(monitor_addr);
    monitor_addr += 4;
    
    lcd_send_command(LCD_LINE2);

    print_page(monitor_addr);
    monitor_addr += 4;

    long_delay();
    long_delay();
  }
}