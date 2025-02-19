#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int PIN_BTN_RED = 28;
const int PIN_BTN_GREEN = 26;
const int PIN_LED_RED = 4;
const int PIN_LED_GREEN = 6;

volatile int flag = 0;
volatile int btn_color = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    if(gpio == PIN_BTN_RED){
      flag = 1;
      btn_color = 0;
    }
  }
  if (events == 0x8){
    if(gpio == PIN_BTN_GREEN){
      flag = 1;
      btn_color = 1;
    }
  }
}

int main() {
  stdio_init_all();

  int red_status = 1;
  int green_status = 1;

  gpio_init(PIN_BTN_RED);
  gpio_set_dir(PIN_BTN_RED, GPIO_IN);
  gpio_pull_up(PIN_BTN_RED);
  gpio_init(PIN_BTN_GREEN);
  gpio_set_dir(PIN_BTN_GREEN, GPIO_IN);
  gpio_pull_up(PIN_BTN_GREEN);
  gpio_init(PIN_LED_RED);
  gpio_set_dir(PIN_LED_RED, GPIO_OUT);
  gpio_init(PIN_LED_GREEN);
  gpio_set_dir(PIN_LED_GREEN, GPIO_OUT);

  gpio_set_irq_enabled_with_callback(PIN_BTN_RED, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
  gpio_set_irq_enabled(PIN_BTN_GREEN, GPIO_IRQ_EDGE_RISE, true);

  while (true) {
    if(flag){
      if(btn_color){
        gpio_put(PIN_LED_GREEN, green_status);
        green_status = !green_status;
      }
      else{
        gpio_put(PIN_LED_RED, red_status);
        red_status = !red_status;
      }
      flag = 0;
    }
  }
}
