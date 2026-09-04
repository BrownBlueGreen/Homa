

#include "kernel.hpp"
#include "list.hpp"

class Semaphore final {
public:
  explicit Semaphore(uint32_t n) : count_(n), maxCount_(n) {
    assert(n > 0 && "counting semaphore needs at least one resource");
  }

  bool give() {
    CriticalSection cs;
    if (count_ == maxCount_) return false;
    count_ += 1;
    kernel.taskUnblock(waiters_);
    return true;
  }

  bool giveFromISR() {
    assert(__get_IPSR() != 0);
    if (count_ == maxCount_) return false;
    count_ += 1;
    kernel.taskUnblockFromISR(waiters_);
    return true;
  }

  // Need to implement timeout == WAIT_FOREVER, as the blocking case 
  bool wait(int32_t timeout) {
    CriticalSection cs;
    uint32_t deadline; 
    if (timeout > 0) deadline = kernel.osTicks_ + static_cast<uint32_t>(timeout);
    while(count_ == 0) {
      if (timeout == 0) return false; // we're not blocking so return false b/c there's no room
      if (timeout > 0) {
        int32_t remaining = static_cast<int32_t>(deadline - kernel.osTicks_);
        if (remaining <= 0) return false;
        kernel.taskBlock(waiters_, remaining);
      } else {
        kernel.taskBlock(waiters_, -1);
      }
      cs.reopen();
      if (kernel.runningTask_->timedOut_) return false;
    }
    count_ -= 1;
    return true;
  }


private:

  uint32_t count_;
  uint32_t maxCount_;
  IntrusiveList<TCB, &TCB::qnext_> waiters_;

};


class BinarySemaphore final {
public:
  explicit Semaphore(uint32_t n) : count_(0), maxCount_(n) {
    assert(n > 0 && "counting semaphore needs at least one resource");
  }

  bool give() {
    CriticalSection cs;
    if (count_ == maxCount_) return false;
    count_ += 1;
    kernel.taskUnblock(waiters_);
    return true;
  }

  bool giveFromISR() {
    assert(__get_IPSR() != 0);
    if (count_ == maxCount_) return false;
    count_ += 1;
    kernel.taskUnblockFromISR(waiters_);
    return true;
  }

  // Need to implement timeout == WAIT_FOREVER, as the blocking case 
  bool wait(int32_t timeout) {
    CriticalSection cs;
    uint32_t deadline; 
    if (timeout > 0) deadline = kernel.osTicks_ + static_cast<uint32_t>(timeout);
    while(count_ == 0) {
      if (timeout == 0) return false; // we're not blocking so return false b/c there's no room
      if (timeout > 0) {
        int32_t remaining = static_cast<int32_t>(deadline - kernel.osTicks_);
        if (remaining <= 0) return false;
        kernel.taskBlock(waiters_, remaining);
      } else {
        kernel.taskBlock(waiters_, -1);
      }
      cs.reopen();
      if (kernel.runningTask_->timedOut_) return false;
    }
    count_ -= 1;
    return true;
  }


private:

  uint32_t count_ = 0;
  uint32_t maxCount_;
  IntrusiveList<TCB, &TCB::qnext_> waiters_;

};

