#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_NUMBER 2

uint8_t pinPort = _SFR_IO_ADDR(PORTD);
uint8_t pinBit = 3;

uint8_t colors[LED_NUMBER][3];

// LSB of each color byte shoud be 0 (otherwise there is timing issue)
void updateLed(uint8_t port, uint8_t pinBit, volatile uint8_t colorsArray[][3], uint8_t size)
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

  DDRD = 1 << 3;

  colors[0][0] = 0b11111110;
  colors[0][1] = 0b11111110;
  colors[0][2] = 0b11111110;
  colors[1][0] = 0b11111110;
  colors[1][1] = 0b11111110;
  colors[1][2] = 0b11111110;

  for (;;)
  {
    updateLed(pinPort, pinBit, colors, LED_NUMBER);

    _delay_ms(50);
  }

  return 0;
}