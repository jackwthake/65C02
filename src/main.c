#define PORTB (*(volatile unsigned char*)0x6000)
#define PORTA (*(volatile unsigned char*)0x6001)
#define DDRB  (*(volatile unsigned char*)0x6002)
#define DDRA  (*(volatile unsigned char*)0x6003)

const unsigned char E = 0b10000000;
const unsigned char RW = 0b01000000;
const unsigned char RS = 0b00100000;

void delay() {
  volatile unsigned int i;
  for (i = 0; i < 60000; ++i) {
    asm("NOP");
  }
}

void send_command(unsigned char cmd) {
  PORTB = cmd;
  delay();

  PORTA = 0;
  delay();
  PORTA = E;
  delay();
  PORTA = 0;
  delay();
}

void print_str(char *str) {
  char c;
  unsigned i = 0;
  
  if (str == 0x00) { 
    return;
  }
  
  while ((c = *(str + i)) != '\0') {
    PORTB = c;

    PORTA = RS;
    delay();
    PORTA = RS | E;
    delay();
    PORTA = RS;

    ++i;
  }
}

void init_display() {
  PORTA = 0;
  DDRB = 0xFF;
  DDRA = 0b11100000;

  send_command(0b00111000); // Set 8-bit mode; 2-line display; 5x8 font
  send_command(0b00001110); // Display on; cursor on; blink off
  send_command(0b00000110); // Increment and shift cursor; don't shift display
}

int main(void) {
  init_display();

  print_str("Hello, World!");

  while(1) {}
  return 0;
}

// issue with stack pointer or somehting 