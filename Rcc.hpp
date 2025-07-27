#include "memory_map.h"

#pragma once 

namespace hal {

class Rcc final {
private: 

    static constexpr uint32_t VALID_AHB1_MASK = 0b01111110111101000001011111111111;
    static constexpr uint32_t VALID_AHB2_MASK = 0b00000000000000000000000011110001;
    static constexpr uint32_t VALID_AHB3_MASK = 0b00000000000000000000000000000001;
    static constexpr uint32_t VALID_APB1_MASK = 0b11110110111111101100100111111111;
    static constexpr uint32_t VALID_APB2_MASK = 0b00000100011101110111111100110011;

    static constexpr uint32_t HPRE_SHIFT  = RCC_CFGR_HPRE_Pos;   // 4
    static constexpr uint32_t PPRE1_SHIFT = RCC_CFGR_PPRE1_Pos;  // 10
    static constexpr uint32_t PPRE2_SHIFT = RCC_CFGR_PPRE2_Pos;

    enum class Bus { AHB1, AHB2, AHB3, APB1, APB2 };
    enum class ClkSrc { HSI, HSE, PLL };
    enum class AHBDiv { 
        Div1   = 0b0000u << RCC_CFGR_HPRE_Pos,  // 0x0000
        Div2   = 0b1000u << RCC_CFGR_HPRE_Pos,  // 0x0080
        Div4   = 0b1001u << RCC_CFGR_HPRE_Pos,  // 0x0090
        Div8   = 0b1010u << RCC_CFGR_HPRE_Pos,  // 0x00A0
        Div16  = 0b1011u << RCC_CFGR_HPRE_Pos,  // 0x00B0
        Div64  = 0b1100u << RCC_CFGR_HPRE_Pos,  // 0x00C0
        Div128 = 0b1101u << RCC_CFGR_HPRE_Pos,  // 0x00D0
        Div256 = 0b1110u << RCC_CFGR_HPRE_Pos,  // 0x00E0
        Div512 = 0b1111u << RCC_CFGR_HPRE_Pos,  // 0x00F0 
    };
    
    enum class APB1Div {
        Div1   = 0b0000u << RCC_CFGR_PPRE1_Pos,  // 0x0000
        Div2   = 0b1000u << RCC_CFGR_PPRE1_Pos,  // 0x0080
        Div4   = 0b1001u << RCC_CFGR_PPRE1_Pos,  // 0x0090
        Div8   = 0b1010u << RCC_CFGR_PPRE1_Pos,  // 0x00A0
        Div16  = 0b1011u << RCC_CFGR_PPRE1_Pos,  // 0x00B0
        Div64  = 0b1100u << RCC_CFGR_PPRE1_Pos,  // 0x00C0
        Div128 = 0b1101u << RCC_CFGR_PPRE1_Pos,  // 0x00D0
        Div256 = 0b1110u << RCC_CFGR_PPRE1_Pos,  // 0x00E0
        Div512 = 0b1111u << RCC_CFGR_PPRE1_Pos,  // 0x00F0 
    }

    enum class APB2Div {
        Div1   = 0b0000u << RCC_CFGR_PPRE2_Pos,  // 0x0000
        Div2   = 0b1000u << RCC_CFGR_PPRE2_Pos,  // 0x0080
        Div4   = 0b1001u << RCC_CFGR_PPRE2_Pos,  // 0x0090
        Div8   = 0b1010u << RCC_CFGR_PPRE2_Pos,  // 0x00A0
        Div16  = 0b1011u << RCC_CFGR_PPRE2_Pos,  // 0x00B0
        Div64  = 0b1100u << RCC_CFGR_PPRE2_Pos,  // 0x00C0
        Div128 = 0b1101u << RCC_CFGR_PPRE2_Pos,  // 0x00D0
        Div256 = 0b1110u << RCC_CFGR_PPRE2_Pos,  // 0x00E0
        Div512 = 0b1111u << RCC_CFGR_PPRE2_Pos,  // 0x00F0 
    }

    template<GPIO_t* Port, uint32_t Pin, uint8_t AF = 0>
    friend class hal::Pin;

    Rcc() = default;

    static inline void configurePll(
        uint32_t M,     // must be in [2..63]
        uint32_t N,     // must be in [192..432]
        uint32_t P,     // one of 2,4,6,8
        uint32_t Q,     // must be in [2..15]
        bool     useHse // false=HSI, true=HSE
    ) {

        /* PUT PARAMETER CHECKS HERE */
        // 1) Turn off PLL and wait for it to go down
        RCC->CR &= ~RCC_CR_PLLON;
        while (RCC->CR & RCC_CR_PLLRDY) {}

        // 2) Pack M and N
        uint32_t pllcfgr = ((M << RCC_PLLCFGR_PLLM_Pos) & RCC_PLLCFGR_PLLM) | ((N << RCC_PLLCFGR_PLLN_Pos) & RCC_PLLCFGR_PLLN);

        // 4) Pack P ‑ use the helper DIV macros
        switch (P) {
            case 2:  
            // P = 2 → bits 17:16 = 00
                break;
            case 4:  
                pllcfgr |= RCC_PLLCFGR_PLLP_0;            // 0x1 << 16
                break;
            case 6:  
                pllcfgr |= RCC_PLLCFGR_PLLP_1;            // 0x2 << 16
                break;
            case 8:  
                pllcfgr |= RCC_PLLCFGR_PLLP_0 | RCC_PLLCFGR_PLLP_1;             // 0x3 << 16
                break;
            default:
                /* error: invalid P */
                break;
        }

        // 5) Pack source
        pllcfgr |= useHse ? RCC_PLLCFGR_PLLSRC_HSE : RCC_PLLCFGR_PLLSRC_HSI;

        // 6) Pack Q
        pllcfgr |= ((Q << RCC_PLLCFGR_PLLQ_Pos) & RCC_PLLCFGR_PLLQ);

        // 7) Write it all at once
        RCC->PLLCFGR = pllcfgr;

        // 8) Re‑enable and wait for lock
        RCC->CR |= RCC_CR_PLLON;
        while (!(RCC->CR & RCC_CR_PLLRDY)) {}
    }
    
    void switchSysClk(Src newSrc, uint8_t flashLatency, AHBDiv ahbDiv, APB1Div apb1Div, APB2Div apb2Div) {
        /* 1. Set the flash latency */
        SET_BIT(FLASH->ACR, flashLatency & FLASH_ACR_LATENCY)

        /* 2. Set AHB/APB prescalers */
        constexpr uint32_t PRESC_MASK = RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2_Msk;
        
        RCC->CFGR = (RCC->CFGR & ~PRESC_MASK)
          | static_cast<uint32_t>(ahbDiv)
          | static_cast<uint32_t>(apb1Div)
          | static_cast<uint32_t>(apb2Div);

        /* 3. Enable and set the new source */
        switch(newSrc) {
            case: ClkSrc::HSI:
                SET_BIT(RCC->CR, RCC_CR_HSION);
                while(!READ_BIT(RCC->CR, RCC_CR_HSIRDY));
                break;

            case: ClkSrc::HSE:
                SET_BIT(RCC->CR, RCC_CR_HSEON);
                while(!READ_BIT(RCC->CR, RCC_CR_HSERDY));
                break;

            case: ClkSrc::PLL:

                configurePll(
                    8,   // M: divides HSE (e.g. 8 MHz) → 1 MHz VCO input
                    336, // N: multiplies → 336 MHz VCO
                    4,   // P: divides → 84 MHz SYSCLK
                    7,   // Q: divides → 48 MHz USB/SDIO
                    ClkSrc::HSE);

                SET_BIT(RCC->CR, RCC_CR_PLLON);
                while(!READ_BIT(RCC->CR, RCC_CR_PLLRDY));
                break;
        }

        /* 4. Switch the source of sysClk and wait for it to take effect */
        uint32_t sw = ((newSrc == ClkSrc::PLL) ? RCC_CFGR_SW_PLL : (newSrc == ClkSrc::HSE) ? RCC_CFGR_SW_HSE : RCC_CFGR_SW_HSI);

        while(READ_BIT(RCC->CFGR,RCC_CFGR_SWS) != sws) {}

        SystemCoreClockUpdate();

    }
    
    // configure the audio PLL (PLLI2S) to generate ~11.2896 MHz for 44.1 kHz ×256:
    template<uint32_t M, uint32_t N, uint32_t R>
    static inline void configurePlli2s() {
        // turn off
        RCC->CR &= ~RCC_CR_PLLI2SON;
        while (RCC->CR & RCC_CR_PLLI2SRDY) {}
        // pack M/N/R into PLLI2SCFGR
        RCC->PLLI2SCFGR = (M << RCC_PLLI2SCFGR_PLLI2SM) | (N << RCC_PLLI2SCFGR_PLLI2SN)| (((R/2)-1) << RCC_PLLI2SCFGR_PLLI2SR);
        // turn on & wait
        RCC->CR |= RCC_CR_PLLI2SON;
        while (!(RCC->CR & RCC_CR_PLLI2SRDY)) {}
    }   

    /// @param M       PLLSAI input divider (2…63)
    /// @param N       PLLSAI VCO multiplier (192…432)
    /// @param P_div   PLLSAI /P output divider (2,4,6 or 8) → SAI‑CLK
    /// @param Q       PLLSAI /Q output divider (2…15) → 48 MHz domains
    /// @param R_div   PLLSAI /R output divider (2,4,6 or 8) → RTC, SDIO, etc.
    static inline void configurePllSai(
        uint32_t M,
        uint32_t N,
        uint32_t P_div,
        uint32_t Q,
        uint32_t R_div
    ) 
    {
        // 1) Disable PLLSAI
        RCC->CR &= ~RCC_CR_PLLSAION;
        while (RCC->CR & RCC_CR_PLLSAIRDY) {}

        // 2) Pack M and N
        uint32_t v = ((M << RCC_PLLSAICFGR_PLLSAIM_Pos) & RCC_PLLSAICFGR_PLLSAIM_Msk)
                | ((N << RCC_PLLSAICFGR_PLLSAIN_Pos) & RCC_PLLSAICFGR_PLLSAIN_Msk);

        // 3) Pack P (00→/2, 01→/4, 10→/6, 11→/8)
        //    using the single‐bit macros for PLLSAIP
        switch (P_div) {
        case 2:  break;  
        case 4:  v |= RCC_PLLSAICFGR_PLLSAIP_0; break;
        case 6:  v |= RCC_PLLSAICFGR_PLLSAIP_1; break;
        case 8:  v |= RCC_PLLSAICFGR_PLLSAIP_0
                    | RCC_PLLSAICFGR_PLLSAIP_1; break;
        default: /* invalid P_div */ return;
        }

        // 4) Pack Q (4‑bit field)
        v |= ((Q << RCC_PLLSAICFGR_PLLSAIQ_Pos) & RCC_PLLSAICFGR_PLLSAIQ_Msk);

        // 5) Pack R (00→/2, 01→/4, 10→/6, 11→/8)
        switch (R_div) {
        case 2:  break;
        case 4:  v |= RCC_PLLSAICFGR_PLLSAIR_0; break;
        case 6:  v |= RCC_PLLSAICFGR_PLLSAIR_1; break;
        case 8:  v |= RCC_PLLSAICFGR_PLLSAIR_0
                    | RCC_PLLSAICFGR_PLLSAIR_1; break;
        default: /* invalid R_div */ return;
        }

        // 6) Write the new configuration in one go
        RCC->PLLSAICFGR = v;

        // 7) Re‑enable PLLSAI and wait until ready
        RCC->CR |= RCC_CR_PLLSAION;
        while (!(RCC->CR & RCC_CR_PLLSAIRDY)) {}
    }
    

    /* LOW SPEED INTERNAL/EXTERNAL (LSI & LSE) CLOCK OSCILLATOR CONTROL */
    /// Select RTC clock source (LSI, LSE or HSE)
    enum class RtcSrc : uint32_t {
        NoClk = 0b00 << RCC_BDCR_RTCSEL_Pos,
        LSE   = 0b01 << RCC_BDCR_RTCSEL_Pos,
        LSI   = 0b10 << RCC_BDCR_RTCSEL_Pos,
        HSE   = 0b11 << RCC_BDCR_RTCSEL_Pos
    };

    static inline void enableLsi() {
        // Turn on LSI
        RCC->CSR |= RCC_CSR_LSION;
        // Wait until ready
        while (!(RCC->CSR & RCC_CSR_LSIRDY)) {}
        return true;
    }

    /// Disable the internal low‑speed RC oscillator
    static inline void disableLsi() {
        RCC->CSR &= ~RCC_CSR_LSION;
        while (RCC->CSR & RCC_CSR_LSIRDY) {}
        return true;
    }

    static inline void enableBackupDomain() {
        PWR->CR1 |= PWR_CR1_DBP;
        // small delay for DBP to settle (if you care)
        __DSB(); 
        __ISB();
    }

    /// Enable the external low‑speed crystal oscillator
    /// @return true when LSERDY goes high
    static inline bool enableLse() {
        enableBackupDomain();
        // Turn on LSE
        RCC->BDCR |= RCC_BDCR_LSION;
        // Wait until ready
        while (!(RCC->BDCR & RCC_BDCR_LSIRDY)) {}
        WRITE_REG(RCC->BDCR, (RCC0->BDCR & !RCC_BDCR_RTCSEL) | static_cast<uint32_t>(RtcSrc::LSI));

        disableBackUpDomainWrite()

        return true;
    }

    /// Disable the external low‑speed crystal oscillator
    /// @return true when LSERDY goes low
    static inline bool disableLse() {
        RCC->BDCR &= ~RCC_BDCR_LSEON;
        while (RCC->BDCR & RCC_BDCR_LSERDY) {}
        return true;
    }

    static inline void selectRtcClock(RtcSrc src) {
        enableBackupDomain();
        // clear RTCSEL bits
        RCC->BDCR &= ~RCC_BDCR_RTCSEL;
        // write new source
        RCC->BDCR |= static_cast<uint32_t>(src);
    }

    /* GPIO PORT CLOCK ENABLE/DISABLE CONTROL */
    /* Compile time version */
    template<uint32_t Mask>
    concept validAHB1Mask = (Mask & ~VALID_AHB1_MASK) == 0;
    template<validAHB1Mask Mask>
    inline void enableAHB1() {
        SET_BIT(RCC->AHB1ENR, Mask);
    }

    template<uint32_t Mask>
    concept validAHB2Mask = (Mask & ~VALID_AHB2_MASK) == 0;
    template<validAHB2Mask Mask>
    inline void enableAHB2() {
        SET_BIT(RCC->AHB2ENR, Mask);
    }

    template<uint32_t Mask>
    concept validAHB3Mask = (Mask & ~VALID_AHB3_MASK) == 0;
    template<validAHB3Mask Mask>
    inline void enableAHB3() {
        SET_BIT(RCC->AHB3ENR, Mask);
    }

    template<uint32_t Mask>
    concept validAPB1Mask = (Mask & ~VALID_APB1_MASK) == 0;
    template<validAPB1Mask Mask>
    inline void enableAPB1() {
        SET_BIT(RCC->APB1ENR, Mask);
    }

    template<uint32_t Mask>
    concept validAPB2Mask = (Mask & ~VALID_APB2_MASK) == 0;
    template<validAPB2Mask Mask>
    inline void enableAPB2() {
        SET_BIT(RCC->APB2ENR, Mask);
    }

    /* Run time version */
    inline void enableAHB1_rt(uint32_t mask) {
        assert((mask & ~VALID_AHB1_MASK) == 0);
        SET_BIT(RCC->AHB1ENR, mask);
    }

    inline void enableAHB2_rt(uint32_t mask) {
        assert((mask & ~VALID_AHB2_MASK) == 0);
        SET_BIT(RCC->AHB2ENR, mask);
    }

    inline void enableAHB3_rt(uint32_t mask) {
        assert((mask & ~VALID_AHB3_MASK) == 0);
        SET_BIT(RCC->AHB3ENR, mask);
    }

    inline void enableAPB1_rt(uint32_t mask) {
        assert((mask & ~VALID_APB1_MASK) == 0);
        SET_BIT(RCC->APB1ENR, mask);
    }

    inline void enableAPB2_rt(uint32_t mask) {
        assert((mask & ~VALID_APB2_MASK) == 0);
        SET_BIT(RCC->APB2ENR, mask);
    }

    // —— Compile‑time version ——

    // AHB1
    template<uint32_t Mask>
    concept validAHB1Mask = (Mask & ~VALID_AHB1_MASK) == 0;
    template<validAHB1Mask Mask>
    inline void disableAHB1() {
        CLEAR_BIT(RCC->AHB1ENR, Mask);
    }

    // AHB2
    template<uint32_t Mask>
    concept validAHB2Mask = (Mask & ~VALID_AHB2_MASK) == 0;
    template<validAHB2Mask Mask>
    inline void disableAHB2() {
        CLEAR_BIT(RCC->AHB2ENR, Mask);
    }

    // AHB3
    template<uint32_t Mask>
    concept validAHB3Mask = (Mask & ~VALID_AHB3_MASK) == 0;
    template<validAHB3Mask Mask>
    inline void disableAHB3() {
        CLEAR_BIT(RCC->AHB3ENR, Mask);
    }

    // APB1
    template<uint32_t Mask>
    concept validAPB1Mask = (Mask & ~VALID_APB1_MASK) == 0;
    template<validAPB1Mask Mask>
    inline void disableAPB1() {
        CLEAR_BIT(RCC->APB1ENR, Mask);
    }

    // APB2
    template<uint32_t Mask>
    concept validAPB2Mask = (Mask & ~VALID_APB2_MASK) == 0;
    template<validAPB2Mask Mask>
    inline void disableAPB2() {
        CLEAR_BIT(RCC->APB2ENR, Mask);
    }

    // —— Runtime version ——

    // AHB1
    inline void disableAHB1_rt(uint32_t mask) {
        assert((mask & ~VALID_AHB1_MASK) == 0);
        CLEAR_BIT(RCC->AHB1ENR, mask);
    }

    // AHB2
    inline void disableAHB2_rt(uint32_t mask) {
        assert((mask & ~VALID_AHB2_MASK) == 0);
        CLEAR_BIT(RCC->AHB2ENR, mask);
    }

    // AHB3
    inline void disableAHB3_rt(uint32_t mask) {
        assert((mask & ~VALID_AHB3_MASK) == 0);
        CLEAR_BIT(RCC->AHB3ENR, mask);
    }

    // APB1
    inline void disableAPB1_rt(uint32_t mask) {
        assert((mask & ~VALID_APB1_MASK) == 0);
        CLEAR_BIT(RCC->APB1ENR, mask);
    }

    // APB2
    inline void disableAPB2_rt(uint32_t mask) {
        assert((mask & ~VALID_APB2_MASK) == 0);
        CLEAR_BIT(RCC->APB2ENR, mask);
    }
}
}