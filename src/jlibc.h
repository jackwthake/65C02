#ifndef __JLIBC__
#define __JLIBC__

#define PORTB (*(volatile unsigned char*)0x6000)
#define PORTA (*(volatile unsigned char*)0x6001)
#define DDRB  (*(volatile unsigned char*)0x6002)
#define DDRA  (*(volatile unsigned char*)0x6003)

#define E 0b10000000
#define RW 0b01000000
#define RS 0b00100000

#define NOP5000() do { \
  asm("ldx #$FF\n1:\ndex\nbne 1b\n" : : : "x"); \
} while(0)

void put_c(char c);
void print_str(const char *str);
void mini_printf(char *fmt, ...);

#endif