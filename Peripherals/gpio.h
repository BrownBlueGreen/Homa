/* 

This is the GPIO driver implemented as a C++ class. What you want is a flexible useful class that isn't too bloated. 

First how is the peripheral set up? there's 6 (I think A - F) GPIO ports and EACH has 32 pins. Now you might be thinking
well that's pretty much ALL the pins of the chip... And here's where you'd be correct. When you want alternate functionality
you would set the alternative function register of said pin to what you want. 
*/

#include "memory_map.h"

#DEFINE MODER 0     /*!< GPIO port mode register,               Address offset: 0x00      */
#DEFINE OTYPER 4    /*!< GPIO port output type register,        Address offset: 0x04      */
#DEFINE OSPEEDR 8   /*!< GPIO port output speed register,       Address offset: 0x08      */
#DEFINE PUPDR 12    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
#DEFINE IDR 16      /*!< GPIO port input data register,         Address offset: 0x10      */
#DEFINE ODR 20      /*!< GPIO port output data register,        Address offset: 0x14      */
#DEFINE BSRR 24     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
#DEFINE LCKR 28     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
#DEFINE AFR_0 32    /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
#DEFINE AFR_1 36 

inline GPIO_t* get_port(uint8_t port) {
  return GPIOA + (0x0400UL * port);
}

class GPIO {
public:

bool GPIO_ENABLE_PORT(uint8_t PORT);

bool GPIO_SET_MODE(uint8_t PORT, uint32_t DATA);
bool GPIO_GET_MODE(uint8_t PORT, uint32_t* DATA);

bool GPIO_SET_OTYPER(uint8_t PORT, uint32_t DATA);
bool GPIO_GET_OTYPER(uint8_t PORT, uint32_t* DATA);

bool GPIO_SET_OSPEEDR(uint8_t PORT, uint32_t DATA);
bool GPIO_GET_OSPEEDR(uint8_t PORT, uint32_t* DATA);

bool GPIO_SET_PUPDR(uint8_t PORT, uint32_t DATA);
bool GPIO_GET_PUPDR(uint8_t PORT, uint32_t* DATA);

bool GPIO_SET_IDR(uint8_t PORT, uint32_t DATA);
bool GPIO_GET_IDR(uint8_t PORT, uint32_t* DATA);

bool GPIO_SET_ODR(uint8_t PORT, uint32_t DATA);
bool GPIO_GET_ODR(uint8_t PORT, uint32_t* DATA);

bool GPIO_SET_BSRR(uint8_t PORT, uint32_t DATA);
bool GPIO_GET_BSRR(uint8_t PORT, uint32_t* DATA);

bool GPIO_SET_LCKR(uint8_t PORT, uint32_t DATA);
bool GPIO_GET_LCKR(uint8_t PORT, uint32_t* DATA);

bool GPIO_SET_AFR(uint8_t PORT, uint32_t LOHI, uint32_t DATA);
bool GPIO_GET_AFR(uint8_t PORT, uint32_t LOHI, uint32_t* DATA);

private:

  GPIO_t* peripheral_;

}