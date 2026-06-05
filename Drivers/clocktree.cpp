


/* NEED TO CREATE A CLASS CALLED CLOCK TREE TO CONTROL THE CLOCK TREE OF THE BOARD */

// static inline void configurePll(uint32_t M, uint32_t N, uint32_t P, uint32_t Q, bool useHse) {
//       /* PUT PARAMETER CHECKS HERE */
//       // 1) Turn off PLL and wait for it to go down
//       RCC->CR &= ~RCC_CR_PLLON;
//       while (RCC->CR & RCC_CR_PLLRDY) {}

//       // 2) Pack M and N
//       uint32_t pllcfgr = ((M << RCC_PLLCFGR_PLLM_Pos) & RCC_PLLCFGR_PLLM) | ((N << RCC_PLLCFGR_PLLN_Pos) & RCC_PLLCFGR_PLLN);

//       // 4) Pack P ‑ use the helper DIV macros
//       switch (P) {
//           case 2:  
//           // P = 2 → bits 17:16 = 00
//               break;
//           case 4:  
//               pllcfgr |= RCC_PLLCFGR_PLLP_0;            // 0x1 << 16
//               break;
//           case 6:  
//               pllcfgr |= RCC_PLLCFGR_PLLP_1;            // 0x2 << 16
//               break;
//           case 8:  
//               pllcfgr |= RCC_PLLCFGR_PLLP_0 | RCC_PLLCFGR_PLLP_1;             // 0x3 << 16
//               break;
//           default:
//               /* error: invalid P */
//               break;
//       }

//       // 5) Pack source
//       pllcfgr |= useHse ? RCC_PLLCFGR_PLLSRC_HSE : RCC_PLLCFGR_PLLSRC_HSI;

//       // 6) Pack Q
//       pllcfgr |= ((Q << RCC_PLLCFGR_PLLQ_Pos) & RCC_PLLCFGR_PLLQ);

//       // 7) Write it all at once
//       RCC->PLLCFGR = pllcfgr;

//       // 8) Re‑enable and wait for lock
//       RCC->CR |= RCC_CR_PLLON;
//       while (!(RCC->CR & RCC_CR_PLLRDY)) {}
//   }
    
//   void switchSysClk(Src newSrc, uint8_t flashLatency, AHBDiv ahbDiv, APB1Div apb1Div, APB2Div apb2Div) {
//     /* 1. Set the flash latency */
//     SET_BIT(FLASH->ACR, flashLatency & FLASH_ACR_LATENCY)

//     /* 2. Set AHB/APB prescalers */
//     constexpr uint32_t PRESC_MASK = RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2_Msk;
    
//     RCC->CFGR = (RCC->CFGR & ~PRESC_MASK) | ahbDiv | apb1Div | apb2Div;

//     /* 3. Enable and set the new source */
//     switch(newSrc) {
//         case: ClkSrc::HSI:
//             SET_BIT(RCC->CR, RCC_CR_HSION);
//             while(!READ_BIT(RCC->CR, RCC_CR_HSIRDY));
//             break;

//         case: ClkSrc::HSE:
//             SET_BIT(RCC->CR, RCC_CR_HSEON);
//             while(!READ_BIT(RCC->CR, RCC_CR_HSERDY));
//             break;

//         case: ClkSrc::PLL:
//             bool pll_uses_hse = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) != 0;
//             configurePll(
//                 8,   // M: divides HSE (e.g. 8 MHz) → 1 MHz VCO input
//                 336, // N: multiplies → 336 MHz VCO
//                 4,   // P: divides → 84 MHz SYSCLK
//                 7,   // Q: divides → 48 MHz USB/SDIO
//                 pll_uses_hse);

//             SET_BIT(RCC->CR, RCC_CR_PLLON);
//             while(!READ_BIT(RCC->CR, RCC_CR_PLLRDY));
//             break;
//     }

//     /* 4. Switch the source of sysClk and wait for it to take effect */
//     uint32_t sw = ((newSrc == ClkSrc::PLL) ? RCC_CFGR_SW_PLL : (newSrc == ClkSrc::HSE) ? RCC_CFGR_SW_HSE : RCC_CFGR_SW_HSI);

//     while(READ_BIT(RCC->CFGR,RCC_CFGR_SWS) != sws) {}

//     SystemCoreClockUpdate();

//   }
    
//   // configure the audio PLL (PLLI2S) to generate ~11.2896 MHz for 44.1 kHz ×256:
//   template<uint32_t M, uint32_t N, uint32_t R>
//   static inline void configurePlli2s() {
//       // turn off
//       RCC->CR &= ~RCC_CR_PLLI2SON;
//       while (RCC->CR & RCC_CR_PLLI2SRDY) {}
//       // pack M/N/R into PLLI2SCFGR
//       RCC->PLLI2SCFGR = (M << RCC_PLLI2SCFGR_PLLI2SM) | (N << RCC_PLLI2SCFGR_PLLI2SN)| (((R/2)-1) << RCC_PLLI2SCFGR_PLLI2SR);
//       // turn on & wait
//       RCC->CR |= RCC_CR_PLLI2SON;
//       while (!(RCC->CR & RCC_CR_PLLI2SRDY)) {}
//   }   

//   /// @param M       PLLSAI input divider (2…63)
//   /// @param N       PLLSAI VCO multiplier (192…432)
//   /// @param P_div   PLLSAI /P output divider (2,4,6 or 8) → SAI‑CLK
//   /// @param Q       PLLSAI /Q output divider (2…15) → 48 MHz domains
//   /// @param R_div   PLLSAI /R output divider (2,4,6 or 8) → RTC, SDIO, etc.
//   static inline void configurePllSai(uint32_t M, uint32_t N, uint32_t P_div, uint32_t Q, uint32_t R_div) {
//       // 1) Disable PLLSAI
//       RCC->CR &= ~RCC_CR_PLLSAION;
//       while (RCC->CR & RCC_CR_PLLSAIRDY) {}

//       // 2) Pack M and N
//       uint32_t v = ((M << RCC_PLLSAICFGR_PLLSAIM_Pos) & RCC_PLLSAICFGR_PLLSAIM_Msk)
//               | ((N << RCC_PLLSAICFGR_PLLSAIN_Pos) & RCC_PLLSAICFGR_PLLSAIN_Msk);

//       // 3) Pack P (00→/2, 01→/4, 10→/6, 11→/8)
//       //    using the single‐bit macros for PLLSAIP
//       switch (P_div) {
//       case 2:  break;  
//       case 4:  v |= RCC_PLLSAICFGR_PLLSAIP_0; break;
//       case 6:  v |= RCC_PLLSAICFGR_PLLSAIP_1; break;
//       case 8:  v |= RCC_PLLSAICFGR_PLLSAIP_0
//                   | RCC_PLLSAICFGR_PLLSAIP_1; break;
//       default: /* invalid P_div */ return;
//       }

//       // 4) Pack Q (4‑bit field)
//       v |= ((Q << RCC_PLLSAICFGR_PLLSAIQ_Pos) & RCC_PLLSAICFGR_PLLSAIQ_Msk);

//       // 5) Pack R (00→/2, 01→/4, 10→/6, 11→/8)
//       switch (R_div) {
//       case 2:  break;
//       case 4:  v |= RCC_PLLSAICFGR_PLLSAIR_0; break;
//       case 6:  v |= RCC_PLLSAICFGR_PLLSAIR_1; break;
//       case 8:  v |= RCC_PLLSAICFGR_PLLSAIR_0
//                   | RCC_PLLSAICFGR_PLLSAIR_1; break;
//       default: /* invalid R_div */ return;
//       }

//       // 6) Write the new configuration in one go
//       RCC->PLLSAICFGR = v;

//       // 7) Re‑enable PLLSAI and wait until ready
//       RCC->CR |= RCC_CR_PLLSAION;
//       while (!(RCC->CR & RCC_CR_PLLSAIRDY)) {}
//   }