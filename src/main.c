#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_NUMBER 1

uint8_t pinPort = _SFR_IO_ADDR(PORTD);
uint8_t pinBit = 3;

volatile uint8_t colors[LED_NUMBER][3];

volatile uint8_t led = 0;
volatile uint8_t ledByte = 0;
volatile uint8_t bit = 0;

ISR(TIMER1_OVF_vect) {

  ICR1 = 0x00FF;

  if((colors[led][ledByte] >> bit++) & 1) {
    OCR1A = 100;
  } else {
    OCR1A = 200;
  }

  if(bit > 7) {
    bit = 0;
    if(++led > 2) {
      ledByte = 0;
      if(++led >= LED_NUMBER) {
        led = 0;
        ICR1 = 0xFFFF;
      }
    }
  }
} 

// LSB of each color byte shoud be 0 (otherwise there is timing issue)
void updateLedASM(uint8_t port, uint8_t pinBit, volatile uint8_t colorsArray[][3], uint8_t size)
{

  uint8_t counter = 0;
  uint8_t bitCounter = 0;
  size = size * 3 + 1;

  uint8_t *a = (uint8_t *)colorsArray;

  PORTD &= 1 << 3;

  asm volatile(
      "cbi %2, %3 \n\t"
      "ldi %0, %4 \n"
    "tx_led_data%=: \n\t"
      "dec %0 \n\t"
    "tx_led_byte%=: \n\t"
      "ldi %1, 0x08 \n\t"
      "ld __tmp_reg__, %a5+ \n\t"
    "tx_led_bit%=: \n\t"
      "breq tx_led_data_end%=\n\t"
      "sbi %2, %3 \n\t"
      "lsl __tmp_reg__ \n\t"
      "brcc set_pin_low%= \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "dec %1 \n\t"
      "rjmp tx_led_bit_end%=\n"
    "set_pin_low%=: \n\t"
      "cbi %2, %3 \n\t"
      "dec %1 \n\t"
      "nop \n\t"
      "nop \n"
    "tx_led_bit_end%=:"
      "cbi %2, %3 \n\t"
      "breq tx_led_data%= \n\t"
      "nop \n\t"
      "brne tx_led_bit%= \n"
    "tx_led_data_end%=:\n\t"
      "clr %5"
      //"sbi %2, %3 \n\t"

      : "=&r"(counter),
        "=&r"(bitCounter)
      : "I"(port),
        "I"(pinBit),
        "I"(size),
        "b"(colorsArray));
}

int main(void)
{

  
  colors[0][0] = 0xFF;
  colors[0][1] = 0b10000100;
  colors[0][2] = 0b10010000;

  sei();

  TCNT1 = 0; //Initialize Timer 1 counter
  DDRB |= (1 << DDB1); //PB1 is an output
    DDRB |= (1 << DDB1); // PB1 is set as an output 

    ICR1 = 0x0FFF; // set PWM counter TOP

    OCR1A = 0x0000; // set 0% PWM dutty cycle

    TCCR1A |= (1 << COM1A1)|(1 << COM1B1); // set non inverting mode

    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12)|(1 << WGM13);
    // set Fast PWM mode using ICR1 as TOP
    
    TCCR1B |= (1 << CS10); // start timer with no prescaler
    TIMSK1 |= (1 << TOIE1); // set intreup on timer overflow  

  while(1){

  }



  for (;;)
  {
    //updateLed(pinPort, pinBit, colors, LED_NUMBER);

    _delay_ms(50);
  }

  return 0;
}