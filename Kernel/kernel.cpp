/* Global kernel object */
#include "kernel.hpp"

using OS = Kernel<8, 128, 8>;
extern OS& kernel;

extern "C" uint32_t*  switchContext(uint32_t* sp) { return kernel.commitSwitch(sp); }
extern "C" uint32_t*  firstTaskStack()            { return kernel.currStackPtr(); }
extern "C" void       taskExitTrap()              { __disable_irq(); for(;;){} }
extern "C" void       SysTick_Handler()           { kernel.onTick(); }

/* This launches the scheduler, meaning it sets up the runningTask to begin execution */
extern "C" [[gnu::naked]] void schedulerLaunch() {
  __asm volatile (
    "BL    firstTaskStack   \n"   // R0 = runningTask_->stack_ptr_
    "ADDS  R0, R0, #32      \n"   // skip the 8 dummy callee-saved words
    "MSR   PSP, R0          \n"   // PSP points at the hardware frame
    "MOVS  R0, #2           \n"   
    "MSR   CONTROL, R0      \n"   // SPSEL=1: Thread mode uses PSP
    "ISB                    \n"   // mandatory after a CONTROL write
    "MOV   R1, #0           \n"
    "MSR   BASEPRI, R1      \n"   // unmask
    "LDR   R0, =0xFFFFFFFD  \n"   // EXC_RETURN: Thread, PSP, basic frame
    "BX    R0               \n"   // hardware unstacks and runs the task
  );
}

/* This PendSV ISR handler is what does the actual context switch */
extern "C" [[gnu::naked]] void PendSV_Handler() {
  __asm volatile (
    "MRS    R0, PSP         \n"
    "STMDB  R0!, {R4-R11}   \n"
    "PUSH   {R3, LR}        \n"
    "BL     switchContext   \n"
    "POP    {R3, LR}        \n"
    "LDMIA  R0!, {R4-R11}   \n"
    "MSR    PSP, R0         \n"
    "BX     LR              \n"
  );
}
