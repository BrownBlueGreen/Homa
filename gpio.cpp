#include "gpio.h"

GPIO::GPIO_ENABLE_PORT(uint8_t p) {
    GPIO_t* gpio_port = get_port(p);

    RCC->AHB1ENR |= port;
}

GPIO::