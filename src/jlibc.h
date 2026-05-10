#ifndef __JLIBC__
#define __JLIBC__

#define PORTB (*(volatile unsigned char*)0x6000)
#define PORTA (*(volatile unsigned char*)0x6001)
#define DDRB  (*(volatile unsigned char*)0x6002)
#define DDRA  (*(volatile unsigned char*)0x6003)

#define LCD_E 0b10000000
#define LCD_RW 0b01000000
#define LCD_RS 0b00100000

#define LCD_INIT_8BIT 0x38        // 8-bit mode, 2-line display, 5x8 font
#define LCD_DISPL_ON 0x0C
#define LCD_CLEAR 0x01            // Display On, Cursor Off, Blink Off.
#define LCD_RETURN_HOME 0x02
#define LCD_DISPL_SHIFT_NORM 0x06 // increment cursor dont shift display
#define LCD_CURS_LEFT 0X10
#define LCD_CURS_right 0X14
#define LCD_SHIFT_LEFT 0X18
#define LCD_SHIFT_right 0X1c

#define NULL 0x00

#define NOP5000() do { \
  asm("ldx #$FF\n1:\ndex\nbne 1b\n" : : : "x"); \
} while(0)

/* 8-bit types */
typedef unsigned char  u8;
typedef signed char    i8;

/* 16-bit types */
typedef unsigned int   u16;
typedef signed int     i16;

/* Size type (usually 16-bit for 64KB address space) */
typedef unsigned int   size_t;

unsigned strnlen(char *str, size_t max);

void long_delay(void);

void lcd_init(void);
void lcd_send_command(unsigned char cmd);
void lcd_clear(void);

void put_c(char c);
void print_str(const char *str);
void print_hex_byte(u8 b);
void print_page(u16 addr);
#endif