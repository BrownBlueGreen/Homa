
#pragma once
#include <cassert>
#include "kernel.hpp"
#include "list.hpp"

class Semaphore final {
public:
  Semaphore(uint32_t initial, uint32_t max) : count_(initial), maxCount_(max) {}
  explicit Semaphore(uint32_t n): count_(n), maxCount_(n) { assert(n > 0); }

  bool give() {
    CriticalSection cs;
    if (count_ == maxCount_) return false;
    count_ += 1;
    SchedulerServices::unblock(waiters_);
    return true;
  }

  bool giveFromISR() {
    assert(__get_IPSR() != 0);
    CriticalSection cs;
    if (count_ == maxCount_) return false;
    count_ += 1;
    SchedulerServices::unblock(waiters_);
    return true;
  }

  // Need to implement timeout == WAIT_FOREVER, as the blocking case 
  bool wait(int32_t timeout) {
    CriticalSection cs;
    uint32_t deadline = 0; 
    if (timeout > 0) deadline = SchedulerServices::ticks() + static_cast<uint32_t>(timeout);
    while(count_ == 0) {
      if (timeout == 0)  return false; // we're not blocking so return false b/c there's no room
      if (timeout > 0)   SchedulerServices::blockUntil(waiters_, deadline); 
      else               SchedulerServices::block(waiters_); 
      cs.reopen();
      if (SchedulerServices::currentTask()->timedOut_) return false;
    }
    count_ -= 1;
    return true;
  }


private:

  uint32_t count_;
  uint32_t maxCount_;
  IntrusiveList<TCB, &TCB::qnext_> waiters_;

};

using BinarySemaphore = Semaphore; // constructed as Semaphore(0, 1)