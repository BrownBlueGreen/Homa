/*
TODO: Add task exit trap - forever loop to execute if a task fails out or returns when it's not supposed to
TODO: Add synchoronization primitives + RUNNING -> BLOCKED FUNCTIONALITY 
*/

#pragma once 
#include <cassert>
#include <cstddef>
#include <cstdint>
#include "list.hpp"

#define BUS_FREQ    16000000
#define KERNEL_PRIO 5

enum class TASKSTATE { READY, RUNNING, BLOCKED, DELAYED };
enum class KERNELSTATE  { UNINITIALIZED, INITIALIZED, RUNNING, STOPPED };

extern "C" {
  uint32_t* switchContext(uint32_t* sp);
  uint32_t* firstTaskStack();
  [[gnu::naked]] void schedulerLaunch();
  void SysTick_Handler();
  void taskExitTrap();
}

struct TCB {
  uint32_t* stack_ptr_;
  TCB*      qnext_;
  TCB*      dnext_;
  TASKSTATE status_;
  uint32_t  priority_;
  uint32_t  burstTime_;
  uint32_t  wakeTick_;
  bool      delayed_;
};

class CriticalSection final {
public:
  CriticalSection() : saved_(__get_BASEPRI()) {
      __set_BASEPRI(KERNEL_PRIO << (8 - __NVIC_PRIO_BITS));
      __DSB(); __ISB();
  }
  ~CriticalSection() { __set_BASEPRI(saved_); }
private:
  uint32_t saved_;
};

template <uint32_t N, uint32_t StackSize, uint32_t MP>
class Kernel final {

  static_assert(KERNEL_PRIO < 14);
  static_assert(StackSize % 2 == 0, "Stack size needs to be even for 8 bit alignment."); /* for stack 8 bit alignment */
  static constexpr uint32_t IDLE_STACK_WORDS = 64;
  static constexpr uint32_t IDLE_PRIORITY = 0xFFFFFFFF;
  
  friend void       ::SysTick_Handler();
  friend uint32_t*  ::switchContext(uint32_t*);
  friend uint32_t*  ::firstTaskStack();
  friend void       ::taskExitTrap();

private: 

  using ReadyList = IntrusiveList<TCB, &TCB::qnext_>; // using qnext pointer for ready and blocked tasks
  using DelayList = IntrusiveList<TCB, &TCB::dnext_>;
  using BlockList = IntrusiveList<TCB, &TCB::qnext_>;


  Kernel() {
    msPrescaler_ = (BUS_FREQ / 1000); /* 1 millisecond tick time */
    SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_SLEEP); /* TODO: Move this to init and mark it with a debug flag */
  }

  TCB                 tasks_[N];                /* Raw array of TCB's to hold the tasks  */
  TCB                 idle_;                    /* Idle task, used when there's nothing to do */
  ReadyList           readyLists_[MP];          /* Array of taskList structs, each array element represents a priority level */
  BlockList           blockedTasks_;            /* taskList struct of blocked tasks */
  DelayList           delayedTasks_;            /* taskList struct of delayed tasks */
  uint32_t            readyMask_      = 0;      /* Bit mask of priorities that have a task(s) */
  uint32_t            msPrescaler_    = 0;
  uint16_t            numTasks_       = 0;
  TCB* volatile       runningTask_    = nullptr;
  TCB* volatile       nextTask_       = nullptr;
  volatile uint32_t   osTicks_        = 0;
  KERNELSTATE         kernelState     = KERNELSTATE::UNINITIALIZED;
  
  alignas(8) uint32_t TCB_STACK[N][StackSize];    /* Stack of all tasks */
  alignas(8) uint32_t idleStack_[IDLE_STACK_WORDS];


  static void idleTaskFunc() { while(1) __WFI(); }

  /* sleep() helper */
  void delayUntil(uint32_t tick) {
    assert(runningTask_ != &idle_);
    runningTask_->wakeTick_ = tick;
    runningTask_->status_   = TASKSTATE::DELAYED; 
    runningTask_->delayed_  = true;
    delayedTasks_.insertSorted(runningTask_, [](const TCB* a, const TCB* b) {
      return (int32_t)(a->wakeTick_ - b->wakeTick_) < 0;
    });
  }

  TCB* popHighestReady() {
    if(!readyMask_) return &idle_;
    
    uint32_t p = __builtin_ctz(readyMask_);
    ReadyList& r = readyLists_[p];
    TCB* t = r.popFront();
    assert(t != nullptr);
    if (r.empty()) readyMask_ &= ~(1U << p);

    return t;
  }

  void pushReady(TCB* t) {
    assert(t != &idle_ && "idle is a fallback, should never be queued!");
    assert(t->priority_ < MP);
    t->status_ = TASKSTATE::READY;
    readyLists_[t->priority_].pushBack(t);
    readyMask_ |= (1U << t->priority_);
  }

  static uint32_t* buildFrame(uint32_t* base, uint32_t words, void(*fn)(void)) {
    base[words - 1] = (1U << 24); // xPSR T-Bit
    base[words - 2] = (uint32_t)fn; // PC register
    base[words - 3] = (uint32_t)taskExitTrap;   // LP fallback
    for (uint32_t k = 4; k <= 16; ++k) base[words - k] = 0xAAAAAAAA;
    return &base[words - 16];
  }

  void initIdleTask() {
    idle_.stack_ptr_  = buildFrame(idleStack_, IDLE_STACK_WORDS, idleTaskFunc);
    idle_.qnext_      = nullptr; 
    idle_.dnext_      = nullptr; 
    idle_.status_     = TASKSTATE::READY;
    idle_.priority_   = IDLE_PRIORITY;
    idle_.burstTime_  = 0;
    idle_.wakeTick_   = 0;
    idle_.delayed_    = false;
  }

  void schedule() {
    TCB* curr = runningTask_;

    if (curr != &idle_ && curr->status_ == TASKSTATE::RUNNING) {
      uint32_t topReady = readyMask_ ? __builtin_ctz(readyMask_) : MP;
      
      if (topReady > curr->priority_) { // Next ready task is lower priority continue running curr
        nextTask_ = curr;
        return;
      } 
      pushReady(curr);
    }

    curr = popHighestReady();
    curr->status_ = TASKSTATE::RUNNING;
    nextTask_ = curr;
  }
 
  void expireDelayed() {
    while(TCB* h = delayedTasks_.front()) {
      if ((int32_t)(osTicks_ - h->wakeTick_) < 0) break;
      delayedTasks_.popFront();
      h->delayed_ = false;
      pushReady(h);
    }
  }
  /* DELAYED -> READY */
  void onTick() {
    osTicks_ += 1;
    expireDelayed();
    schedule()
    if(runningTask_ != nextTask_) {
      SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
      __DSB();
      __ISB();
    }
  }

  uint32_t* commitSwitch(uint32_t* sp) {
    runningTask_->stack_ptr_ = sp;
    runningTask_ = nextTask_;
    return runningTask_->stack_ptr_;
  }

  uint32_t* launchStackPtr() { return runningTask_->stack_ptr_; }

  void taskYield() {
    schedule();
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
    __DSB();
    __ISB();
  }
  /* NEED TO IMPLEMENT BLOCKED -> READY PATH, THIS HAPPENS WHEN 1. WRITING TO QUEUE, 2. SEM GIVE, 3. MUTEX GIVE */

public:

  /* Deleted constructors */
  Kernel(const Kernel&)             = delete;
  Kernel(Kernel&&)                  = delete;

  Kernel& operator=(const Kernel&)  = delete;
  Kernel& operator=(Kernel&&)       = delete;

  
  void init   () {
    if (kernelState != KERNELSTATE::UNINITIALIZED) return;
    kernelState = KERNELSTATE::INITIALIZED;
  }

  void launch (uint32_t quanta) {
    if(kernelState != KERNELSTATE::INITIALIZED) return;
    if (quanta > (0x1000000u / msPrescaler_)) return; /* Check for SysTick LOAD overflow */
    if (numTasks_ == 0) return;

    __set_BASEPRI(KERNEL_PRIO << (8 - __NVIC_PRIO_BITS));
    __DSB();
    __ISB();

    initIdleTask();

    runningTask_ = popHighestReady();
    runningTask_->status_ = TASKSTATE::RUNNING;
    nextTask_ = runningTask_;
    
    kernelState = KERNELSTATE::RUNNING;

    /* Reset systick, clear the value register and load the quanta */
    WRITE_REG(SysTick->CTRL, 0);
    WRITE_REG(SysTick->VAL, 0);
    WRITE_REG(SysTick->LOAD, quanta * msPrescaler_ - 1);

    /* Set the SysTick and PendSV priorities */
    NVIC_SetPriority(SysTick_IRQn, 14);
    NVIC_SetPriority(PendSV_IRQn, 15);

    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk);
    schedulerLaunch();
    
  }

  void addTask (void(*task_func)(void), uint32_t i, uint32_t priority, uint32_t burst_time) {
    if (numTasks_ >= N || i >= N) return;
    if (priority >= MP) return;
    if (kernelState != KERNELSTATE::INITIALIZED) return;

    CriticalSection cs;
    
    tasks_[i].stack_ptr_  = buildFrame(TCB_STACK[i], StackSize, task_func);
    tasks_[i].qnext_      = nullptr; 
    tasks_[i].dnext_      = nullptr; 
    tasks_[i].status_     = TASKSTATE::READY;
    tasks_[i].priority_   = priority;
    tasks_[i].burstTime_  = burst_time;
    tasks_[i].wakeTick_   = 0;
    tasks_[i].delayed_    = false;

    pushReady(&tasks_[i]);

    numTasks_ += 1;
  }


  /* RUNNING -> DELAYED */
  void sleep(uint32_t ms) {
    CriticalSection cs;
    if(ms == 0) {
      taskYield();
      return;
    }
    delayUntil(osTicks_ + ms);
    taskYield();
  }

  static Kernel& getInstance() { 
    static Kernel instance; 
    return instance; 
  }

};

