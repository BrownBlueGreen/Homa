#include "Base/system_mmap.h"

/* DEFAULT YOU WANT TIMEOUT TO OCCUR EVERY 1 SECOND */
/* DEFAULT CLOCK IS 16MHZ */
#define ONE_SEC_LOAD  16000000

#define CTRL_ENABLE (1U<<0)
#define CTRL_TICKINT (1U<<1)
#define CTRL_CLKSRC (1U<<2)
#define CTRL_COUNTFLAG (1U<<16)
#define MAX_DELAY 0xFFFFFFFFU

volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_prime;
volatile uint32_t tick_freq = 1;

void tick_increment() {
  g_curr_tick += tick_freq;
}

/* Delay in seconds */
void delay(uint32_t delay) {
  uint32_t tickstart = get_tick();
  uint32_t wait = delay;

  if (wait < MAX_DELAY) {
    wait += static_cast<uint32_t>(tick_freq);
  }

  while((get_tick() - tickstart) < wait) {}
}

uint32_t get_tick() {

  __disable_irq();
  g_curr_tick_prime = g_curr_tick;
  __enable_irq();

  return g_curr_tick_prime;
}

/* This function configures systick to fire and interrupt every second */
void timebase_init(void) {
  /* Reload the timer with number of cycles per second */
  SysTick->LOAD = ONE_SEC_LOAD - 1;
  
  /* Clear systick current value register */
  WRITE_REG(SysTick->VAL, 0);
  
  /* Select internal clock source */
  WRITE_REG(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);
  
  /* Enable interrupt */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
  
  /* Enable systick */
  SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

  /* Enable global interrupts */
  __enable_irq();
}

// THIS ONE WAS COMMENTED OUT FOR THE OTHER ONE IN KERNEL
// void SysTick_Handler() {
//   tick_increment();
// }

template<TIM_t* Timer, Pin* pin> 
class Timer {
public:

  Timer() = default;

private:
}