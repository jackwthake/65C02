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

void init_display(void);
void print_str(const char *str);

void __attribute__((interrupt)) irq(void) {
  *(volatile unsigned char*)0x600D = 0x20;
}

void send_command(unsigned char cmd) {
  PORTB = cmd;

  PORTA = 0;
  PORTA = E;
  NOP5000();
  PORTA = 0;
}

void print_str(const char *str) {
  char c;
  unsigned i = 0;
  
  if (str == 0x00) { 
    return;
  }
  
  while ((c = *(str + i)) != '\0') {
    PORTB = c;

    PORTA = RS;
    PORTA = RS | E;
    NOP5000();
    PORTA = RS;

    ++i;
  }
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

  print_str("Hello, World!!");

  unsigned char pattern = 0b10101010;
  while(1) {
    unsigned long int i;
    PORTB = pattern;
    pattern = (pattern << 1) | (pattern >> 7);
    
    for (i = 0; i < 12000; ++i) { asm("NOP"); }
  }
}