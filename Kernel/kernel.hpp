#pragma once 

#define NUM_THREADS 4
#define STACK_SIZE  100
#define BUS_FREQ    16000000
#define PERIOD      100 // period is 100 * quanta, eg quanta = 10 implies 100 * 10 = 1000
uint32_t MILLIS_PRESCALER;
uint32_t period_tick;

void osSchedulerLaunch();
void osScheulderRoundRobin();
void tim2_1hz_interrupt_init();



TCB tcbs[NUM_THREADS];
TCB* curr_thread_ptr;

int32_t TCB_STACK[NUM_THREADS][STACKSIZE];

void osSchedulerLaunch(void) {
  /* LOAD ADDRESS OF CURRENT_THREAD_PT INTO R0 */
  __asm("LDR R0, =curr_thread_ptr");

  /* LOAD R2 FROM ADDRESS = R0 */
  __asm("LDR R2, [R0]");

  /* LOAD CORTEX-M STACK POINTER FROM R2, ie. SP = curr_thread_ptr->stack_ptr */
  __asm("LDR SP, [R2]");

  /* RESTORE MANUAL REGISTERS */
  __asm("POP {R4-R11}");

  /* RESTORE REGISTER R0 to R12, THIS IS REALLY "LAUNCHING" THE SCHEDULER */
  __asm("POP {R0-R12}");

  /* SKIP LR AND PSR */
  __asm("ADD SP, SP, #4");

  /* SP NOW POINTS TO PC, READ THAT AND PUT IT INTO LR */
  __asm("POP {LR}");

  /* SKIP PSR BY ADDING 4 AGAIN */
  __asm("ADD SP, SP, #4");

  /* ENABLE INTERRUPTS */
  __asm("CPSIE  I");

  /* RETURN FROM EXCEPTION TO LR */
  __asm("BX   LR");

}  

/* Initialize the stacks of all threads */
void osKernelStackInitForThread(int i) {

  /* First initialize the stack pointer */
  tcbs[i].stack_ptr_ = &TCB_STACK[i][STACK_SIZE - 16];

  /* 
  Set bit 21 of PSR register to 1 to set it to thumb mode. 
  */
  TCB_STACK[i][STACK_SIZE - 1] = (1U << 24);

  /* 
  For each thread you want to initialize the stack frame this means you put dummy values 
  in all the locations. Up to position 8 is the actual stack frame. Setting up a dummy frame
  is optional though. 
  */
  TCB_STACK[i][STACK_SIZE - 3] = 0xAAAAAAAA; /* R14 i.e. LR register */
  TCB_STACK[i][STACK_SIZE - 4] = 0xAAAAAAAA; /* R12 */
  TCB_STACK[i][STACK_SIZE - 5] = 0xAAAAAAAA; /* R3 */
  TCB_STACK[i][STACK_SIZE - 6] = 0xAAAAAAAA; /* R2 */
  TCB_STACK[i][STACK_SIZE - 7] = 0xAAAAAAAA; /* R1 */
  TCB_STACK[i][STACK_SIZE - 8] = 0xAAAAAAAA; /* R0 */

  /* These registers are not preserved during a context switch. */
  TCB_STACK[i][STACK_SIZE - 9] = 0xAAAAAAAA; /* R11 */
  TCB_STACK[i][STACK_SIZE - 10] = 0xAAAAAAAA; /* R10 */
  TCB_STACK[i][STACK_SIZE - 11] = 0xAAAAAAAA; /* R9 */
  TCB_STACK[i][STACK_SIZE - 12] = 0xAAAAAAAA; /* R8 */
  TCB_STACK[i][STACK_SIZE - 13] = 0xAAAAAAAA; /* R7 */
  TCB_STACK[i][STACK_SIZE - 14] = 0xAAAAAAAA; /* R6 */
  TCB_STACK[i][STACK_SIZE - 15] = 0xAAAAAAAA; /* R5 */
  TCB_STACK[i][STACK_SIZE - 16] = 0xAAAAAAAA; /* R4 */

}

// uint8_t osKernelAddThreads(void(*task0)(void),void(*task1)(void),void(*task2)(void),void(*task3)(void),) {

//   /* disable global interrupts */
//   __disable_irq();
//   tcbs[0].next_ptr_ = &tcbs[1];
//   tcbs[1].next_ptr_ = &tcbs[2];
//   tcbs[2].next_ptr_ = &tcbs[3];
//   tcbs[3].next_ptr_ = &tcbs[0];

//   /* Initialize PC by setting entry point and initialize stack for thread 0 */
//   osKernelStackInit(0);
//   TCB_STACK[0][STACK_SIZE - 2] = (int32_t)task0;

//   /* Initialize PC by setting entry point and initialize stack for thread 0 */
//   osKernelStackInit(1);
//   TCB_STACK[1][STACK_SIZE - 2] = (int32_t)task1;

//   /* Initialize PC by setting entry point and initialize stack for thread 0 */
//   osKernelStackInit(2);
//   TCB_STACK[2][STACK_SIZE - 2] = (int32_t)task2;

//   /* Initialize PC by setting entry point and initialize stack for thread 0 */
//   osKernelStackInit(3);
//   TCB_STACK[3][STACK_SIZE - 2] = (int32_t)task3;

//   /* start from thread 0 */
//   curr_thread_ptr = &tcbs[0];

//   /* Enable global interrupts */
//   __enable_irq();

//   return 1;
// }

void osKernelInit() {
  MILLIS_PRESCALER = (BUS_FREQ / 1000); /* 1 millisecond tick time */

}

// void osKernelLaunch(uin32_t quanta) {

//   /* Reset systick */
//   SysTick->CTRL = 0;

//   /* Clear systick value register */
//   SysTick->VAL = 0;

//   /* Load quanta */
//   SysTick->LOAD = (quanta * MILLIS_PRESCALER);

//   /* Set systick to low priority */
//   NVIC_SetPriority(SysTick_IRQn, 15);

//   /* Enable systick, select internal clock */
//   SysTick->CTRL = (1U << 0) | (1U << 2);

//   /* Enable systick interrupt */
//   SysTick->CTRL |= (1U << 1);

//   /* Launch scheduler */
//   osSchedulerLaunch();

// }

void osThreadYield(void) {
  /* Clear Systick Current Value register */
  SysTick->VAL = 0;

  /* 
  SET THE SYSTICK PENDING ISR BIT IN THE APPROPRIATE CORE REGISTER 
  THIS TRIGGERS SYSTICK HANDLER AND CONTEXT SWITCH 
  */
  SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;

}

/* What if you have a task you want executed periodically? Like every 500ms you want this to run? Periodic Scheduler. 
Check if the period has been reached, so you can execute periodic function before the next task. Periodic threads are unlike regular tasks. 
But a periodic thread does not have an infinite loop. 
*/

/* PERIODIC SCHEDULER TASK SWITCH FUNCTION */
void osSchedulerRoundRobin() {
  if((++period_tick) == PERIOD) {
    (*task4)();
    period_tick = 0;
  }

  curr_thread_ptr = current_thread_ptr->next_ptr_
}

// /* This */
// void tim2_1hz_interrupt_init() {

//   /* ENABLE CLOCK ACCESS TO TIM2 */
//   RCC->APB1EN |= (1U << 0);

//   /* SET TIMER PRESCALER */
//   TIM2->PSC = 1600 - 1; // default clock is 16mhz, so interrupt every 10 ms

//   /* SET AUTORELOAD VALUE */
//   TIM2->ARR = 10000 - 1;

//   /* CLEAR THE TIMER COUNTER */
//   TIM2->CNT = 0;

//   /* ENABLE TIMER */
//   TIM2->CR1 = (1U << 0);

//   /* ENABLE TIMER INTERRUPT */
//   TIM2->DIER |= (1U << 0);

//   /* ENABLE TIMER INTERRUPT IN NVIC */
//   NVIC_EnableIRQ(TIM2_IRQn);
// }


/* 
WE DON'T WANT THE STACK TO CHANGE DURING EXECUTION OF SYSTICK INTERRUPT ROUTINES 
FIND MORE INFO IN GNU DOCS
*/

/* THIS IS THE MEAT AND BONES OF THE CONTEXT SWITCH */
// __attribute__((naked)) void SysTick_Handler(void) {
//   /* 1. SUSPEND THE CURRENT THREAD */

//   /* DISABLE GLOBAL INTERRUPTS */
//   __asm("CPSID  I");

//   /* SAVE R4, R5, R6, R7, R8, R9, R10, R11 ONTO THE STACK */
//   __asm("PUSH {R4-R11}");

//   /* LOAD ADDRESS OF curr_thread_ptr INTO R0 */
//   __asm("LDR R0, =curr_thread_ptr");

//   /* LOAD R1 from address equals R0, ie, r1 = current thread ptr */
//   __asm("LDR R1,[R0]");

//   /* STORE CORTEX-M SP at address = R1, ie. SAVE SP INTO TCB */
//   __asm("STR SP,[R1]");

//   /* 2. CHOOSE THE NEXT THREAD */

//   /* LOAD R1 FROM A LOCATION 4 BYTES ABOVE R1, ie, R1 = current_thread_ptr->next */
//   __asm("LDR R1,[R1,4]");

//   /* STORE R1 AT ADDRESS = R0 ie current-thread-ptr = r1, WE'RE UPDATING THE CURRENT_THREAD_PTR */
//   __asm("STR R1, [R0]");

//   /*  LOAD THE CORTEX-M STACK POINTER SP FROM ADDRESS = R1, SP = current_thread_ptr -> stackPt */
//   __asm("LDR SP,[R1]");

//   /* IF YOU WANT PERIODIC TASK CALL, REPLACE THE 3 LINES ABOVE WITH: 
  // __asm("PUSH   {R0, LR}");
  // __asm("BL     osSchedulerRoundRobin");
  // __asm("POP    {R0, LR}");

//   /* R1 = currentPt ie. new thread
//   __asm("LDR    R1, [R0]");

//   /* SP = currentPt->stackPt
//   __asm("LDR    SP, [R1]");
//   */

//   /* LOAD REGISTERS CONSISTING OF THIS THREADS STACK FRAME */
//   __asm("POP {R4-R11}");

//   /* ENABLE GLOBAL INTERRUPTS */
//   __asm("CPSIE  I");

//   /* RETURN FROM EXCEPTION AND RESTORE STACK FRAME */
//   __asm("BX    LR")

// }

/* SEMAPHORE PART */
void osSemaphoreInit(int32_t* semaphore, int32_t value) {
  *semaphore = value;
}

void osSemaphoreSet(int32_t* semaphore) {
  /* DISABLE INTERRUPT BECAUSE THIS IS SUPPOSED TO BE ATOMIC */
  __disable_irq();

  *semaphore += 1;

  __enable_irq();
}

void osSemaphoreWait(int32_t* semaphore) {
  
  while(*sempahore <= 0) {
    __disable_irq();
    __enable_irq();
  }

  __disable_irq();

  *sempahore -= 1;

  __enable_irq();
}

/* NEED TO MAKE GLOBAL KERNEL OBJECT */

/* NOW DEFINE THE TIM2 INTERRUPT HANDLER AND THIS IS CALLED BY THE INTERRUPT */
extern "C" void TIM2_IRQHandler() {
  /* 1. CLEAR INTERRUPT FLAG */
  CLEAR_BIT(TIM2->SR, TIM_SR_UIF);

  /* 2. KERNEL TICK */
  kernel.onOsTick();

  /* 5. CHECK FOR CONTEXT SWITCH AND SET PENDSV BIT */
  if(kernel.schedule()) {
    SET_BIT(SCB->ICSR, SCB_ICSR_PENDSVSET_Msk);
  }
}


extern "C" [[gnu::naked]] void PendSV_Handler() {
  /* 1. SUSPEND THE CURRENT THREAD */

  /* DISABLE GLOBAL INTERRUPTS */
  __asm("CPSID  I");

  /* SAVE R4, R5, R6, R7, R8, R9, R10, R11 ONTO THE STACK */
  __asm("PUSH {R4-R11}");

  /* LOAD ADDRESS OF curr_thread_ptr INTO R0 */
  __asm("LDR R0, =curr_thread_ptr");

  /* LOAD R1 from address equals R0, ie, r1 = current thread ptr */
  __asm("LDR R1,[R0]");

  /* STORE CORTEX-M SP at address = R1, ie. SAVE SP INTO TCB */
  __asm("STR SP,[R1]");

  /* 2. CHOOSE THE NEXT THREAD */

  /* LOAD R1 FROM A LOCATION 4 BYTES ABOVE R1, ie, R1 = current_thread_ptr->next */
  __asm("LDR R1,[R1,4]");

  /* STORE R1 AT ADDRESS = R0 ie current-thread-ptr = r1, WE'RE UPDATING THE CURRENT_THREAD_PTR */
  __asm("STR R1, [R0]");

  /*  LOAD THE CORTEX-M STACK POINTER SP FROM ADDRESS = R1, SP = current_thread_ptr -> stackPt */
  __asm("LDR SP,[R1]");

  /* LOAD REGISTERS CONSISTING OF THIS THREADS STACK FRAME */
  __asm("POP {R4-R11}");

  /* ENABLE GLOBAL INTERRUPTS */
  __asm("CPSIE  I");

  /* RETURN FROM EXCEPTION AND RESTORE STACK FRAME */
  __asm("BX    LR")
}

enum class Status { READY, RUNNING, WAITING, BLOCKED };

struct TCB {
  uint32_t* stack_ptr_;
  TCB*      next_;
  Status    status_;
  uint32_t  period_;
  uint32_t  burst_time_;
  uint8_t   priority_;
  uint32_t  remainingTicks_;
}

template <SchedulerPolicy P, uint32_t N, uint32_t StackSize, uint32_t MaxPriorities>
class Kernel final {

private: 
  Kernel() {
    for(int i, i < N; i++) {
      tasks[i].next_ = &tasks[(i + 1) % N];
    }

    msPrescaler = (BUS_FREQ / 1000); /* 1 millisecond tick time */
  }

  ~Kernel() = default;

  TCB tasks[N];
  TCB* readyQueue_[N];
  TCB* blockedQueue_[N];
  TCB* waitingQueue_[N];
  TCB* nextTask_ = nullptr;
  TCB* runningTask_ = nullptr;

  TBC_STACK[N][StackSize];

  State kernelState     = State::Uninitialized;
  uint32_t osTicks      = 0;
  uint16_t numTasks     = 0;
  uint16_t msPrescaler  = 0;

public:
  Kernel(const Kernel&)             = delete;
  Kernel& operator=(const Kernel&)  = delete;
  Kernel(Kernel&&)                  = delete;
  Kernel& operator=(Kernel&&)       = delete;

  enum class State { Uninitialized, Initialized, Running };

  [[gnu::naked]] void schedulerLaunch() {
    /* LOAD ADDRESS OF CURRENT_THREAD_PT INTO R0 */
    __asm("LDR R0, =runningThread_");

    /* LOAD R2 FROM ADDRESS = R0 */
    __asm("LDR R2, [R0]");

    /* LOAD CORTEX-M STACK POINTER FROM R2, ie. SP = runningThread_->stack_ptr */
    __asm("LDR SP, [R2]");

    /* RESTORE MANUAL REGISTERS */
    __asm("POP {R4-R11}");

    /* RESTORE REGISTER R0 to R12, THIS IS REALLY "LAUNCHING" THE SCHEDULER */
    __asm("POP {R0-R12}");

    /* SKIP LR AND PSR */
    __asm("ADD SP, SP, #4");

    /* SP NOW POINTS TO PC, READ THAT AND PUT IT INTO LR */
    __asm("POP {LR}");

    /* SKIP PSR BY ADDING 4 AGAIN */
    __asm("ADD SP, SP, #4");

    /* ENABLE INTERRUPTS */
    __asm("CPSIE  I");

    /* RETURN FROM EXCEPTION TO LR */
    __asm("BX   LR");
  }  

  void launch(uint32_t quanta) {
    if(kernelState != State::Initialized) return;
    
    /* ENABLE CLOCK ACCESS TO TIM2 */
    SET_BIT(RCC->APB1EN, RCC_APB1RSTR_TIM2RST);

    /* SET TIMER PRESCALER */
    WRITE_REG(TIM2->PSC, 160 - 1);

    /* SET AUTORELOAD VALUE */
    WRITE_REG(TIM2->ARR, quanta * (BUS_FREQ / TIM2->PSC));

    /* CLEAR THE TIMER COUNTER */
    WRITE_REG(TIM2->CNT, 0);

    /* ENABLE TIMER */
    SET_BIT(TIM2->CR1, TIM_CR1_CEN);

    /* ENABLE TIMER INTERRUPT */
    SET_BIT(TIM2->DIER, TIM_DIER_UIE);

    /* SET INTERRUPT PRIORITIES */
    NVIC_SetPriority(TIM2_IRQn, 14);
    NVIC_SetPriority(PendSV_IRQn, 15);

    /* ENABLE TIMER INTERRUPT IN NVIC */
    NVIC_EnableIRQ(TIM2_IRQn);

    osSchedulerLaunch();

    kernelState = State::Running;

  }

  void init() {
    if (kernelState != State::Uninitialized) return;
    kernelState = State::Initialized;
  }

  /* Function creates a singleton instance of the class */
  static Kernel& getInstance(uint32_t quanta) {
    static Kernel() instance;
    return instance;
  }

  void createTask(void(*task)(void), uint32_t i, uint32_t priority, uint32_t burst_time, bool isStart = false) {
    if (i >= N) return;
    if (p >= MaxPriorities) p = MaxPriorities;
    if (kernelState != State::Initialized) return;

    __disable_irq();
    
    /* Initialize PC by setting entry point and initialize stack */
    osKernelStackInit(i);
    TCB_STACK[i][STACK_SIZE - 2] = (int32_t)task;

    tasks[i].status_ = Status::READY;
    tasks[i].priority_ = priority;
    tasks[i].burst_time_ = burst_time;

    if (isStart) {
      runningThread_ = (int32_t)task;
    }

    numTasks += 1;

    /* Enable global interrupts */
    __enable_irq();

  }

  /* This function initializes the stack for a task i */
  void initializeTaskStack(uint32_t i) {
    if (i >= N) return;

    /* First initialize the stack pointer */
    tasks[i].stack_ptr_ = &TCB_STACK[i][STACK_SIZE - 16];

    /* 
    Set bit 21 of PSR register to 1 to set it to thumb mode. 
    */
    TCB_STACK[i][STACK_SIZE - 1] = (1U << 24);

    /* 
    For each thread you want to initialize the stack frame this means you put dummy values 
    in all the locations. Up to position 8 is the actual stack frame. Setting up a dummy frame
    is optional though. 
    */
    TCB_STACK[i][STACK_SIZE - 3] = 0xAAAAAAAA; /* R14 i.e. LR register */
    TCB_STACK[i][STACK_SIZE - 4] = 0xAAAAAAAA; /* R12 */
    TCB_STACK[i][STACK_SIZE - 5] = 0xAAAAAAAA; /* R3 */
    TCB_STACK[i][STACK_SIZE - 6] = 0xAAAAAAAA; /* R2 */
    TCB_STACK[i][STACK_SIZE - 7] = 0xAAAAAAAA; /* R1 */
    TCB_STACK[i][STACK_SIZE - 8] = 0xAAAAAAAA; /* R0 */

    /* These registers are not preserved during a context switch. */
    TCB_STACK[i][STACK_SIZE - 9] = 0xAAAAAAAA; /* R11 */
    TCB_STACK[i][STACK_SIZE - 10] = 0xAAAAAAAA; /* R10 */
    TCB_STACK[i][STACK_SIZE - 11] = 0xAAAAAAAA; /* R9 */
    TCB_STACK[i][STACK_SIZE - 12] = 0xAAAAAAAA; /* R8 */
    TCB_STACK[i][STACK_SIZE - 13] = 0xAAAAAAAA; /* R7 */
    TCB_STACK[i][STACK_SIZE - 14] = 0xAAAAAAAA; /* R6 */
    TCB_STACK[i][STACK_SIZE - 15] = 0xAAAAAAAA; /* R5 */
    TCB_STACK[i][STACK_SIZE - 16] = 0xAAAAAAAA; /* R4 */
  }

  void onOsTick() {
    osTicks += 1;
    // Update tasks --> May need to change things like update waiting tasks and such 
  }

}
