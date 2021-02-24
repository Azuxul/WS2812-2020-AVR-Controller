#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_NUMBER 2  // THE NUMBER OF LEDS SHOUD BE EQUAL TO THE REAL NUMBER OF LEDS OTHERWISE THE WRONG COLORS WILL BE DSIPLAYED 

volatile uint8_t colors[LED_NUMBER][3];

uint8_t ledPinPort = _SFR_IO_ADDR(PORTD);
uint8_t ledPinBit = 3;

// Update WS2812-2020 LEDs colors, max LEDs per line : 85 
// Target CPU frequency : 16MHz
void updateLedASM(uint8_t port, uint8_t pinBit, uint8_t colorsArray[][3], uint8_t size)
{

  volatile uint8_t counter = 0;
  volatile uint8_t bitCounter = 0;
  size = size * 3;

  PORTD &= 1 << 3;

  asm volatile(
      "cbi %2, %3 \n\t"
      "ldi %0, %4 \n"
    "tx_led_data%=: \n\t"

      "sbi %2, %3 \n\t"
      "ld __tmp_reg__, %a5+ \n\t" 
      "lsl __tmp_reg__ \n\t"
      "brcs .+4 \n\t"
      "cbi %2, %3 \n\t"
      "brcc .+7 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "cbi %2, %3 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"

      "sbi %2, %3 \n\t"
      "nop \n\t"
      "lsl __tmp_reg__ \n\t"
      "brcs .+4 \n\t"
      "cbi %2, %3 \n\t"
      "brcc .+8 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "cbi %2, %3 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"

      "sbi %2, %3 \n\t"
      "nop \n\t"
      "lsl __tmp_reg__ \n\t"
      "brcs .+4 \n\t"
      "cbi %2, %3 \n\t"
      "brcc .+8 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "cbi %2, %3 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"

      "sbi %2, %3 \n\t"
      "nop \n\t"
      "lsl __tmp_reg__ \n\t"
      "brcs .+4 \n\t"
      "cbi %2, %3 \n\t"
      "brcc .+8 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "cbi %2, %3 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"

      "sbi %2, %3 \n\t"
      "nop \n\t"
      "lsl __tmp_reg__ \n\t"
      "brcs .+4 \n\t"
      "cbi %2, %3 \n\t"
      "brcc .+8 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "cbi %2, %3 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"

      "sbi %2, %3 \n\t"
      "nop \n\t"
      "lsl __tmp_reg__ \n\t"
      "brcs .+4 \n\t"
      "cbi %2, %3 \n\t"
      "brcc .+8 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "cbi %2, %3 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"

      "sbi %2, %3 \n\t"
      "nop \n\t"
      "lsl __tmp_reg__ \n\t"
      "brcs .+4 \n\t"
      "cbi %2, %3 \n\t"
      "brcc .+8 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "cbi %2, %3 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"

      "sbi %2, %3 \n\t"
      "nop \n\t"
      "lsl __tmp_reg__ \n\t"
      "brcs .+4 \n\t"
      "cbi %2, %3 \n\t"
      "brcc .+8 \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "nop \n\t"
      "cbi %2, %3 \n\t"

      "dec %0 \n\t"
      "breq tx_led_data_end%=\n\t"
      "rjmp tx_led_data%=\n\t"
    "tx_led_data_end%=:\n\t"
      "clr %5 \n\t"
      "cbi %2, %3 \n\t"

      : "=&r"(counter),
        "=&r"(bitCounter)
      : "I"(port),
        "I"(pinBit),
        "I"(size),
        "b"(colorsArray));
}

int main(void)
{

  colors[0][0] = 0b00000000;
  colors[0][1] = 0b00000000;
  colors[0][2] = 0b11111111; 
  colors[1][0] = 0b00000000;
  colors[1][1] = 0b11111111;
  colors[1][2] = 0b00001111; 

  DDRD = 1 << 3;

  for (;;)
  {
    updateLedASM(ledPinPort, ledPinBit, colors, LED_NUMBER);

    _delay_ms(1);
    
  }

  return 0;
}