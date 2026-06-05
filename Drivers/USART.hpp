#include "system_mmap.h"

namespace hal {

template<Pin Tx, Pin Rx, USART_t* USARTx>
class USART {
public:


    USART(uint32_t br, bool dir) : ....

    static void init(uint32_t freqHz) {

        /* 1. Configure the Tx and Rx pins to alternate function mode (AF07) */

        /* 2. Enable clock access to the USARTx module */

        /* 3. initialize the tx pin */

    }

    static void configure() {

    }

private:

    uint32_t buadRate_;
    bool direction_;
    bool enabled_;
    USART_t* module;

}

}