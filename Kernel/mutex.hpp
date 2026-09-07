#include "kernel.hpp"
#include "list.hpp"
#include <cassert>

class Mutex final {

public:

  Mutex() = default;

  /* wait forever. TODO: Need to add restorepriority when taskdeletion is added. */
  void lock() {
    assertTaskContext();
    CriticalSection cs;
    auto curr = kernel.currentTask();
    assert(owner_ != curr && "recursive lock on a non-recursive index");
    
    while(owner_) {
      /* Need to raise priority of owner is the task attempting to lock is higher */
      if (owner_->priority_ > curr->priority_) {
        // priority of the owner is lower than current task
        kernel.raisePriority(owner_, curr->priority_);
      }
      
      kernel.taskBlock(waiters_);
      cs.reopen();
    }
    
    owner_ = curr;
    savedPriority_ = owner_->priority_;
  }

  /* one shot attempt */
  bool tryLock() {
    assertTaskContext();
    CriticalSection cs;
    auto curr = kernel.currentTask();
    if (!owner_) {
      owner_ = curr;
      savedPriority_ = owner_->priority_;
      return true;
    }
    return false;
  }

  /* Wait for mutex for some time, timeout = 0 defaults to tryLock */
  bool tryLockFor(int32_t timeout) {
    /* Delegate */
    if (timeout == 0) { return tryLock(); }
    if (timeout < 0)  { lock(); return true; }

    assertTaskContext();
    CriticalSection cs;
    auto curr = kernel.currentTask();

    uint32_t deadline = kernel.ticks() + static_cast<uint32_t>(timeout);
    while(owner_) {
      if(owner_->priority_ > curr->priority_) {
        kernel.raisePriority(owner_, curr->priority_);
      }

      kernel.taskBlockUntil(waiters_, deadline);
      cs.reopen();
      if(curr->timedOut_ == true) {
        reevaluateBoost();
        return false;
      }
    }

    /* Acquire the lock */
    owner_ = curr;
    savedPriority_ = owner_->priority_;
    return true;
  }

  /* Trying to release an acquired mutex returns false */
  bool release() {
    assertTaskContext();
    CriticalSection cs;
    auto curr = kernel.currentTask();

    if (owner_ != curr) return false; /* Not the owner */
    if (owner_->priority_ != savedPriority_) {
      kernel.restorePriority(owner_, savedPriority_);
    }

    owner_ = nullptr;
    kernel.taskUnblock(waiters_);
    return true;
  }

private:
  static void assertTaskContext() {
    assert(__get_IPSR() == 0 && "mutex operations are not valid from an ISR");
  }

  /* reevaluate priority */
  void reevaluateBoost() {
    if(!owner_) return;
    uint32_t target = savedPriority_;
    if (TCB* head = waiters_.front()) {
      if (head->priority_ < target) target = head->priority_;
    }

    kernel.setPriority(owner_, target);
  }

  uint32_t savedPriority_ = 0;
  TCB* owner_ = nullptr;
  IntrusiveList<TCB, &TCB::qnext_> waiters_;
  
};