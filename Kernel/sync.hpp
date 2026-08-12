
/* PERIODIC SCHEDULER TASK SWITCH FUNCTION */
void osSchedulerRoundRobin() {
  if((++period_tick) == PERIOD) {
    (*task4)();
    period_tick = 0;
  }

  curr_thread_ptr = current_thread_ptr->next_ptr_
}


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
