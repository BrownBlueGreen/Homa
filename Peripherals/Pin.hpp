#include "system.h"
#include "memory_map.h"

#pragma once 
namespace hal {
template<GPIO_t* Port, uint32_t Pin, uint8_t AF = 0>
class Pin {
public:

    enum class Mode     : uint32_t { Input = 0, Output, AltFn, Analog };
    enum class Pull     : uint32_t { None = 0, Up, Down };
    enum class Speed    : uint32_t { Low = 0, Med, Fast, High };
    enum class outType  : uint32_t { pushPull = 0, openDrain };

    static inline void enableClock() {
        if (Port == GPIOA) hal::Rcc.enableAhb1(0x1U << 0);
        if (Port == GPIOB) hal::Rcc.enableAhb1(0x1U << 1);
        if (Port == GPIOC) hal::Rcc.enableAhb1(0x1U << 2);
        if (Port == GPIOD) hal::Rcc.enableAhb1(0x1U << 3);
        if (Port == GPIOE) hal::Rcc.enableAhb1(0x1U << 4);
        if (Port == GPIOF) hal::Rcc.enableAhb1(0x1U << 5);
        if (Port == GPIOG) hal::Rcc.enableAhb1(0x1U << 6);
        if (Port == GPIOH) hal::Rcc.enableAhb1(0x1U << 7);
        if (Port == GPIOI) hal::Rcc.enableAhb1(0x1U << 8);
        if (Port == GPIOJ) hal::Rcc.enableAhb1(0x1U << 9);
        if (Port == GPIOK) hal::Rcc.enableAhb1(0x1U << 10);
    }

    static inline void setMode(Mode m) {

        constexpr uint32_t pos = Pin * 2;
        uint32_t val = static_cast<uint32_t>(m);

        /* 1. Clear the MODER bits */
        CLEAR_BIT(Port->MODER, (0x3U << pos));
        /* 2. Set the correct pin bits */
        SET_BIT(Port->MODER, ((val & 0x1U) << pos) | ((val & 0x2U) << (pos + 1)));

    }

    static inline void setPull(Pull p) {
        constexpr uint32_t pos = Pin * 2;
        uint32_t val = static_cast<uint32_t>(p);

        /* 1. Clear the PUPDR bits */
        CLEAR_BIT(Port->PUPDR, (0x3U << pos));
        /* 2. Set the correct pin bits */
        SET_BIT(Port->PUPDR, ((val & 0x1U) << pos) | ((val & 0x2U) << (pos + 1)));

    }

    static inline void setSpeed(Speed s) {
        constexpr uint32_t pos = Pin * 2;
        uint32_t val = static_cast<uint32_t>(s);

        /* 1. Clear the PUPDR bits */
        CLEAR_BIT(Port->OSPEEDR, (0x3U << pos));
        /* 2. Set the correct pin bits */
        SET_BIT(Port->OSPEEDR, ((val & 0x1U) << pos) | ((val & 0x2U) << (pos + 1)));
    }

    static inline void setOutType(outType oT) {
        constexpr uint32_t pos = Pin * 2;
        uint32_t val = static_cast<uint32_t>(oT);

        /* 1. Clear the PUPDR bits */
        CLEAR_BIT(Port->OTYPER, (0x1U << pos));
        /* 2. Set the correct pin bits */
        SET_BIT(Port->OTYPER, ((val & 0x1U) << pos));
    }

    static inline uint32_t getInputData() {
        return READ_REG(Port->IDR);
    }

    static inline void setOutData(bool val) {
        constexpr uint32_t pos = Pin * 2;
        SET_BIT(Port->ODR, val << pos)
    }

    static inline uint8_t getOutData() {
        constexpr uint32_t pos = Pin * 2;
        READ_BIT(Port->ODR, 0x1U << pos)
    }

    static inline void writeBSRR(bool high) {
        SET_BIT(Port->BSRR, high ? (0x1U << Pin) : (0x1U << (Pin + 16)));
    }

    static inline bool lockPin() {
        constexpr uint32_t LCKK = (0x1U << 16);
        constexpr uint32_t pos = (0x1U << Pin);
        
        /* 1. Set the pin you want to lock */
        SET_PIN(Port->LCKR, pos);

        /* 2. Start the lock sequence */
        SET_BIT(Port->LCKR, LCKK);
        CLEAR_BIT(Port->LCKR, LCKK);
        SET_BIT(Port->LCKR, LCKK);
        READ_BIT(Port->LCKR, LCKK);

        /* 3. Return bit value for lock confirmation */
        return READ_BIT(Port->LCKR, LCKK);
    }

    static inline void configAltFn(uint32_t altFunc) {
        /* 1. Get AFR index */
        uint32_t AFR_pos = Pin / 4;
        uint32_t AFR_idx = Pin / 8;

        /* 3. Clear alt func bits */
        CLEAR_BIT(Port->AFR[AFR_idx], 0b1111 << AFR_pos);

        SET_BIT(Port->AFR[AFR_idx], 
            ((altFunc & 0x1U) << AFR_pos) |
            ((altFunc & 0x2U) << (AFR_pos + 1)) |
            ((altFunc & 0x4U) << (AFR_pos + 2)) |
            ((altFunc & 0x8U) << (AFR_pos + 3))
        );
    }
}
}