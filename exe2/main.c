#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN = 28;
const int LED_PIN = 4;
volatile int flag = 0;

void btn_callback(uint gpio, uint32_t events) {
  if(events == 0x4){
    flag = 1;
  }
}

int main() {
  stdio_init_all();
  int led_status = 1;

  gpio_set_irq_enabled_with_callback(BTN_PIN, 
    GPIO_IRQ_EDGE_FALL, 
    true,
    &btn_callback);

  gpio_init(BTN_PIN);
  gpio_set_dir(BTN_PIN, GPIO_IN);
  gpio_pull_up(BTN_PIN);
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  while (true) {
    if(flag){
      led_status = !led_status;
      gpio_put(LED_PIN, led_status);
      flag = 0;
    }
  }
}
