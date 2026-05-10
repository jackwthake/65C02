#include "jlibc.h"

int main(void) {
  lcd_init();
  u16 monitor_addr = 0x8000; // tryin to set this as volatile seems to mess with initialisation
                             // if set to volatile its initial value is never set to 0x8000
  
  while (1) {
    lcd_clear();

    print_page(monitor_addr); // for some reason always prints memory at 0x8000
    monitor_addr += 4;
    
    lcd_send_command(0xC0); // move to next line

    print_page(monitor_addr); // prints correct scrolling value
    monitor_addr += 4;


    long_delay();
    long_delay();
  }
}