#include "system_mmap.h"

#pragma once 

namespace hal {

enum class Bus { AHB1, AHB2, AHB3, APB1, APB2 };
enum class ClkSrc { HSI, HSE, PLL };
enum class AHBDiv : uint32_t {
    Div1   = RCC_CFGR_HPRE_DIV1,
    Div2   = RCC_CFGR_HPRE_DIV2,
    Div4   = RCC_CFGR_HPRE_DIV4,
    Div8   = RCC_CFGR_HPRE_DIV8,
    Div16  = RCC_CFGR_HPRE_DIV16,
    Div64  = RCC_CFGR_HPRE_DIV64,
    Div128 = RCC_CFGR_HPRE_DIV128,
    Div256 = RCC_CFGR_HPRE_DIV256,
    Div512 = RCC_CFGR_HPRE_DIV512,
};

enum class APB1Div {
    Div1   = RCC_CFGR_PPRE1_DIV1,
    Div2   = RCC_CFGR_PPRE1_DIV2,
    Div4   = RCC_CFGR_PPRE1_DIV4,
    Div8   = RCC_CFGR_PPRE1_DIV8,
    Div16  = RCC_CFGR_PPRE1_DIV16,
};

enum class APB2Div {
    Div1   = RCC_CFGR_PPRE2_DIV1,
    Div2   = RCC_CFGR_PPRE2_DIV2,
    Div4   = RCC_CFGR_PPRE2_DIV4,
    Div8   = RCC_CFGR_PPRE2_DIV8,
    Div16  = RCC_CFGR_PPRE2_DIV16,
};

static constexpr uint32_t VALID_AHB1_MASK = 0b01111110111101000001011111111111;
static constexpr uint32_t VALID_AHB2_MASK = 0b00000000000000000000000011110001;
static constexpr uint32_t VALID_AHB3_MASK = 0b00000000000000000000000000000001;
static constexpr uint32_t VALID_APB1_MASK = 0b11110110111111101100100111111111;
static constexpr uint32_t VALID_APB2_MASK = 0b00000100011101110111111100110011;


class Rcc final {
private: 
  Rcc() = default;

  static inline bool pll_ready() { return RCC->CR & RCC_CR_PLLRDY; }
  static inline uint32_t sysclk_status() { return RCC->CFGR & RCC_CFGR_SWS; }

  /* HSI CLOCK CONTROL */
  static bool enableHsi(uint32_t timeout) {
    SET_BIT(RCC->CR, RCC_CR_HSION);
    if (timeout == 0) return true;

    while ((RCC->CR & RCC_CR_HSIRDY) == 0) {
      if (--timeout == 0) return false;
    }
    return true;
  }

  static bool disableHsi(uint32_t timeout) {
    CLEAR_BIT(RCC->CR, RCC_CR_HSION);
    if (timeout == 0) return true;
    
    while ((RCC->CR & RCC_CR_HSIRDY) == 0) {
      if (--timeout == 0) return false;
    }
    return true;
  }

  static bool trimHsi(uint8_t trim) {
    static_assert(trim <= 0xF);
    SET_BIT(RCC->CR, RCC_CR_)
  }

  /* HSE CLOCK CONTROL */
  static bool enableHse(uint32_t timout) {
    // Turn on HSE
    SET_BIT(RCC->CR, RCC_CR_HSEON);
    if (timeout == 0) return true;
    
    while ((RCC->CR & RCC_CR_HSERDY) == 0) {
      if (--timeout == 0) return false;
    }
    return true;
  }

  static bool disableHse(uint32_t timeout) {
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON);
    if (timeout == 0) return true;
    
    while ((RCC->CR & RCC_CR_HSERDY) == 0) {
      if (--timeout == 0) return false;
    }
    return true;
  }

  static void enableHseBypass(bool en) {
    assert((RCC->CR & RCC_CR_HSEON) == 0);
    if (en) SET_BIT(RCC->CR, RCC_CR_HSEBYP);
    else    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
  }

  /* LSI CLOCK CONTROL */
  static bool enableLsi(uint32_t timeout) {
    // Turn on LSI
    SET_BIT(RCC->CSR, RCC_CSR_LSION);
    if (timeout == 0) return true;
    // Wait until ready
    while (!(RCC->CSR & RCC_CSR_LSIRDY)) {
      if (--timeout == 0) return false;
    }
    return true;
  }

  static bool disableLsi(uint32_t timeout) {
    CLEAR_BIT(RCC->CSR, RCC_CSR_LSION);
    if (timeout == 0) return true;
    
    while (RCC->CSR & RCC_CSR_LSIRDY) {
      if (--timeout == 0) return false;
    }
    return true;
  }

  /* GPIO PORT CLOCK ENABLE/DISABLE CONTROL */
  /* Functions do the following:
  1. ASSERT BITS TO BE SET/CLEARED ARE VALID
  2. SET/CLEAR BITS
  3. DUMMY READY TO SPEND A COUPLE CYCLES AND MAKE SURE PERIPHERAL HAS CLOCK ENABLED
  */
  template<uint32_t Mask>
  static void enableAHB1() {
    static_assert((Mask & ~VALID_AHB1_MASK) == 0, "Invalid AHB1 mask");
    SET_BIT(RCC->AHB1ENR, Mask);
    (void)RCC->AHB1ENR;
  }

  template<uint32_t Mask>
  static void enableAHB2() {
    static_assert((Mask & ~VALID_AHB2_MASK) == 0, "Invalid AHB2 mask");
    SET_BIT(RCC->AHB2ENR, Mask);
    (void)RCC->AHB1ENR;
  }

  template<uint32_t Mask>
  static void enableAHB3() {
    static_assert((Mask & ~VALID_AHB3_MASK) == 0, "Invalid AHB3 mask");
    SET_BIT(RCC->AHB3ENR, Mask);
    (void)RCC->AHB1ENR;
  }

  template<uint32_t Mask>
  static void enableAPB1() {
    static_assert((Mask & ~VALID_APB1_MASK) == 0, "Invalid APB1 mask");
    SET_BIT(RCC->APB1ENR, Mask);
    (void)RCC->AHB1ENR;
  }

  template<uint32_t Mask>
  static void enableAPB2() {
    static_assert((Mask & ~VALID_APB2_MASK) == 0, "Invalid APB2 mask");
    SET_BIT(RCC->APB2ENR, Mask);
    (void)RCC->AHB1ENR;
  }

  /* DISABLE FUNCTION */
  template<uint32_t Mask>
  static void disableAHB1() {
    static_assert((Mask & ~VALID_AHB1_MASK) == 0, "Invalid AHB1 mask");
    CLEAR_BIT(RCC->AHB1ENR, Mask);
  }

  // AHB2
  template<uint32_t Mask>
  static void disableAHB2() {
    static_assert((Mask & ~VALID_AHB2_MASK) == 0, "Invalid AHB2 mask");
    CLEAR_BIT(RCC->AHB2ENR, Mask);
  }

  // AHB3
  template<uint32_t Mask>
  static void disableAHB3() {
    static_assert((Mask & ~VALID_AHB3_MASK) == 0, "Invalid AHB3 mask");
    CLEAR_BIT(RCC->AHB3ENR, Mask);
  }

  // APB1
  template<uint32_t Mask>
  static void disableAPB1() {
    static_assert((Mask & ~VALID_APB1_MASK) == 0, "Invalid APB1 mask");
    CLEAR_BIT(RCC->APB1ENR, Mask);
  }

  // APB2
  template<uint32_t Mask>
  static void disableAPB2() {
    static_assert((Mask & ~VALID_APB2_MASK) == 0, "Invalid APB2 mask");
    CLEAR_BIT(RCC->APB2ENR, Mask);
  }
}

}