#include "jlibc.h"

void __attribute__((interrupt)) irq(void) {
  *(volatile unsigned char*)0x600D = 0x20;
}

void put_c(char c) {
  PORTB = c;

  PORTA = RS;
  PORTA = RS | E;
  NOP5000();
  PORTA = RS;
}

void mini_printf(char *fmt, ...) {
  // In many 8-bit compilers, the first variadic argument 
  // is physically located right after the last named argument in memory.
  char **arg_ptr = (char **)(&fmt + 1); 

  char *p = fmt;
  while (*p != '\0') {
    if (*p == '%') {
      p++;
      if (*p == 's') {
        char *str_arg = *arg_ptr;
        print_str(str_arg);
        arg_ptr++; // Move to the next 2-byte pointer
      }
    } else {
      put_c(*p);
    }

    p++;
  }
}

void print_str(const char *str) {
  char c;
  unsigned i = 0;
  
  if (str == 0x00) { 
    return;
  }
  
  while ((c = *(str + i)) != '\0') {
    put_c(c);

    ++i;
  }
}