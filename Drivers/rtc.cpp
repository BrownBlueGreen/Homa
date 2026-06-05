  /* 
  LOW SPEED INTERNAL/EXTERNAL (LSI & LSE) CLOCK OSCILLATOR CONTROL 
  Select RTC clock source (LSI, LSE or HSE)
  */
    /* LSE CLOCK CONTROL */
  static inline void enableLse() {
    enableBackupDomain();
    // Turn on LSE
    RCC->BDCR |= RCC_BDCR_LSEON;
    // Wait until ready
    while (!(RCC->BDCR & RCC_BDCR_LSERDY)) {}
    WRITE_REG(RCC->BDCR, (RCC->BDCR & ~RCC_BDCR_RTCSEL) | static_cast<uint32_t>(RtcSrc::LSE));

    disableBackUpDomainWrite()
  }

  /// Disable the external low‑speed crystal oscillator
  /// @return true when LSERDY goes low
  static inline void disableLse() {
    RCC->BDCR &= ~RCC_BDCR_LSEON;
    while (RCC->BDCR & RCC_BDCR_LSERDY) {}
  }
  enum class RtcSrc : uint32_t {
      NoClk = 0b00 << RCC_BDCR_RTCSEL_Pos,
      LSE   = 0b01 << RCC_BDCR_RTCSEL_Pos,
      LSI   = 0b10 << RCC_BDCR_RTCSEL_Pos,
      HSE   = 0b11 << RCC_BDCR_RTCSEL_Pos
  };
  

static inline void selectRtcClock(RtcSrc src) {
    enableBackupDomain();
    // clear RTCSEL bits
    RCC->BDCR &= ~RCC_BDCR_RTCSEL;
    // write new source
    RCC->BDCR |= static_cast<uint32_t>(src);
}

  static inline void enableBackupDomain() {
    PWR->CR1 |= PWR_CR1_DBP;
    // small delay for DBP to settle (if you care)
    __DSB(); 
    __ISB();
  }