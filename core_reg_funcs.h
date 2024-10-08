


#ifndef __CORE_REG_FUNCS_H
#define __CORE_REG_FUNCS_H

#include "homa_base.h"

/* ignore some GCC warnings */
#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


/* ###########################  Core Function Access  ########################### */
/** \ingroup  Core_FunctionInterface
    \defgroup Core_FunctionInterface Core Register Access Functions
  @{
 */

/**
  \brief   Enable IRQ Interrupts
  \details Enables IRQ interrupts by clearing the I-bit in the CPSR.
           Can only be executed in Privileged modes.
 */
[[gnu::always_inline]] static inline void __enable_irq(void)
{
  asm volatile ("cpsie i" : : : "memory");
}


/**
  \brief   Disable IRQ Interrupts
  \details Disables IRQ interrupts by setting the I-bit in the CPSR.
  Can only be executed in Privileged modes.
 */
[[gnu::always_inline]] static inline void __disable_irq(void)
{
  asm volatile ("cpsid i" : : : "memory");
}


/**
  \brief   Get Control Register
  \details Returns the content of the Control Register.
  \return               Control Register value
 */
[[gnu::always_inline]] static inline uint32_t __get_CONTROL(void)
{
  uint32_t result;

  asm volatile ("MRS %w0, control" : "=r" (result) );
  return(result);
}


/**
  \brief   Set Control Register
  \details Writes the given value to the Control Register.
  \param [in]    control  Control Register value to set
 */
[[gnu::always_inline]] static inline void __set_CONTROL(uint32_t control)
{
  asm volatile ("MSR control, %w0" : : "r" (control) : "memory");
}


/**
  \brief   Get IPSR Register
  \details Returns the content of the IPSR Register.
  \return               IPSR Register value
 */
[[gnu::always_inline]] static inline uint32_t __get_IPSR(void)
{
  uint32_t result;

  asm volatile ("MRS %w0, ipsr" : "=r" (result) );
  return(result);
}


/**
  \brief   Get APSR Register
  \details Returns the content of the APSR Register.
  \return               APSR Register value
 */
[[gnu::always_inline]] static inline uint32_t __get_APSR(void)
{
  uint32_t result;

  asm volatile ("MRS %w0, apsr" : "=r" (result) );
  return(result);
}


/**
  \brief   Get xPSR Register
  \details Returns the content of the xPSR Register.

    \return               xPSR Register value
 */
[[gnu::always_inline]] static inline uint32_t __get_xPSR(void)
{
  uint32_t result;

  asm volatile ("MRS %w0, xpsr" : "=r" (result) );
  return(result);
}


/**
  \brief   Get Process Stack Pointer
  \details Returns the current value of the Process Stack Pointer (PSP).
  \return               PSP Register value
 */
[[gnu::always_inline]] static inline uint32_t __get_PSP(void)
{
  uint32_t result;

  asm volatile ("MRS %w0, psp\n"  : "=r" (result) );
  return(result);
}


/**
  \brief   Set Process Stack Pointer
  \details Assigns the given value to the Process Stack Pointer (PSP).
  \param [in]    topOfProcStack  Process Stack Pointer value to set
 */
[[gnu::always_inline]] static inline void __set_PSP(uint32_t topOfProcStack)
{
  asm volatile ("MSR psp, %w0\n" : : "r" (topOfProcStack) : "sp");
}


/**
  \brief   Get Main Stack Pointer
  \details Returns the current value of the Main Stack Pointer (MSP).
  \return               MSP Register value
 */
[[gnu::always_inline]] static inline uint32_t __get_MSP(void)
{
  uint32_t result;

  asm volatile ("MRS %w0, msp\n" : "=r" (result) );
  return(result);
}


/**
  \brief   Set Main Stack Pointer
  \details Assigns the given value to the Main Stack Pointer (MSP).

    \param [in]    topOfMainStack  Main Stack Pointer value to set
 */
[[gnu::always_inline]] static inline void __set_MSP(uint32_t topOfMainStack)
{
  asm volatile ("MSR msp, %w0\n" : : "r" (topOfMainStack) : "sp");
}


/**
  \brief   Get Priority Mask
  \details Returns the current state of the priority mask bit from the Priority Mask Register.
  \return               Priority Mask value
 */
[[gnu::always_inline]] static inline uint32_t __get_PRIMASK(void)
{
  uint32_t result;

  asm volatile ("MRS %w0, primask" : "=r" (result) );
  return(result);
}


/**
  \brief   Set Priority Mask
  \details Assigns the given value to the Priority Mask Register.
  \param [in]    priMask  Priority Mask
 */
[[gnu::always_inline]] static inline void __set_PRIMASK(uint32_t priMask)
{
  asm volatile ("MSR primask, %w0" : : "r" (priMask) : "memory");
}


#if       (__CORTEX_M >= 0x03U)

/**
  \brief   Enable FIQ
  \details Enables FIQ interrupts by clearing the F-bit in the CPSR.
           Can only be executed in Privileged modes.
 */
[[gnu::always_inline]] static inline void __enable_fault_irq(void)
{
  asm volatile ("cpsie f" : : : "memory");
}


/**
  \brief   Disable FIQ
  \details Disables FIQ interrupts by setting the F-bit in the CPSR.
           Can only be executed in Privileged modes.
 */
[[gnu::always_inline]] static inline void __disable_fault_irq(void)
{
  asm volatile ("cpsid f" : : : "memory");
}


/**
  \brief   Get Base Priority
  \details Returns the current value of the Base Priority register.
  \return               Base Priority register value
 */
[[gnu::always_inline]] static inline uint32_t __get_BASEPRI(void)
{
  uint32_t result;

  asm volatile ("MRS %w0, basepri" : "=r" (result) );
  return(result);
}


/**
  \brief   Set Base Priority
  \details Assigns the given value to the Base Priority register.
  \param [in]    basePri  Base Priority value to set
 */
[[gnu::always_inline]] static inline void __set_BASEPRI(uint32_t value)
{
  asm volatile ("MSR basepri, %w0" : : "r" (value) : "memory");
}


/**
  \brief   Set Base Priority with condition
  \details Assigns the given value to the Base Priority register only if BASEPRI masking is disabled,
           or the new value increases the BASEPRI priority level.
  \param [in]    basePri  Base Priority value to set
 */
[[gnu::always_inline]] static inline void __set_BASEPRI_MAX(uint32_t value)
{
  asm volatile ("MSR basepri_max, %w0" : : "r" (value) : "memory");
}


/**
  \brief   Get Fault Mask
  \details Returns the current value of the Fault Mask register.
  \return               Fault Mask register value
 */
[[gnu::always_inline]] static inline uint32_t __get_FAULTMASK(void)
{
  uint32_t result;

  asm volatile ("MRS %w0, faultmask" : "=r" (result) );
  return(result);
}


/**
  \brief   Set Fault Mask
  \details Assigns the given value to the Fault Mask register.
  \param [in]    faultMask  Fault Mask value to set
 */
[[gnu::always_inline]] static inline void __set_FAULTMASK(uint32_t faultMask)
{
  asm volatile ("MSR faultmask, %w0" : : "r" (faultMask) : "memory");
}

#endif /* (__CORTEX_M >= 0x03U) */


#if       (__CORTEX_M == 0x04U) || (__CORTEX_M == 0x07U)

/**
  \brief   Get FPSCR
  \details Returns the current value of the Floating Point Status/Control register.
  \return               Floating Point Status/Control register value
 */
[[gnu::always_inline]] static inline uint32_t __get_FPSCR(void)
{
#if (__FPU_PRESENT == 1U) && (__FPU_USED == 1U)
  uint32_t result;

  /* Empty asm statement works as a scheduling barrier */
  asm volatile ("");
  asm volatile ("VMRS %w0, fpscr" : "=r" (result) );
  asm volatile ("");
  return(result);
#else
   return(0);
#endif
}


/**
  \brief   Set FPSCR
  \details Assigns the given value to the Floating Point Status/Control register.
  \param [in]    fpscr  Floating Point Status/Control value to set
 */
[[gnu::always_inline]] static inline void __set_FPSCR(uint32_t fpscr)
{
#if (__FPU_PRESENT == 1U) && (__FPU_USED == 1U)
  /* Empty asm statement works as a scheduling barrier */
  asm volatile ("");
  asm volatile ("VMSR fpscr, %w0" : : "r" (fpscr) : "vfpcc");
  asm volatile ("");
#endif
}

#endif /* (__CORTEX_M == 0x04U) || (__CORTEX_M == 0x07U) */



/*@} end of CMSIS_Core_RegAccFunctions */


/* ##########################  Core Instruction Access  ######################### */
/** \defgroup CMSIS_Core_InstructionInterface CMSIS Core Instruction Interface
  Access to dedicated instructions
  @{
*/

/* Define macros for porting to both thumb1 and thumb2.
 * For thumb1, use low register (r0-r7), specified by constraint "l"
 * Otherwise, use general registers, specified by constraint "r" */
#if defined (__thumb__) && !defined (__thumb2__)
#define __CMSIS_GCC_OUT_REG(r) "=l" (r)
#define __CMSIS_GCC_USE_REG(r) "l" (r)
#else
#define __CMSIS_GCC_OUT_REG(r) "=r" (r)
#define __CMSIS_GCC_USE_REG(r) "r" (r)
#endif

/**
  \brief   No Operation
  \details No Operation does nothing. This instruction can be used for code alignment purposes.
 */
[[gnu::always_inline]] static inline void __NOP(void)
{
  asm volatile ("nop");
}


/**
  \brief   Wait For Interrupt
  \details Wait For Interrupt is a hint instruction that suspends execution until one of a number of events occurs.
 */
[[gnu::always_inline]] static inline void __WFI(void)
{
  asm volatile ("wfi");
}


/**
  \brief   Wait For Event
  \details Wait For Event is a hint instruction that permits the processor to enter
    a low-power state until one of a number of events occurs.
 */
[[gnu::always_inline]] static inline void __WFE(void)
{
  asm volatile ("wfe");
}


/**
  \brief   Send Event
  \details Send Event is a hint instruction. It causes an event to be signaled to the CPU.
 */
[[gnu::always_inline]] static inline void __SEV(void)
{
  asm volatile ("sev");
}


/**
  \brief   Instruction Synchronization Barrier
  \details Instruction Synchronization Barrier flushes the pipeline in the processor,
           so that all instructions following the ISB are fetched from cache or memory,
           after the instruction has been completed.
 */
[[gnu::always_inline]] static inline void __ISB(void)
{
  asm volatile ("isb 0xF":::"memory");
}


/**
  \brief   Data Synchronization Barrier
  \details Acts as a special kind of Data Memory Barrier.
           It completes when all explicit memory accesses before this instruction complete.
 */
[[gnu::always_inline]] static inline void __DSB(void)
{
  asm volatile ("dsb 0xF":::"memory");
}


/**
  \brief   Data Memory Barrier
  \details Ensures the apparent order of the explicit memory operations before
           and after the instruction, without ensuring their completion.
 */
[[gnu::always_inline]] static inline void __DMB(void)
{
  asm volatile ("dmb 0xF":::"memory");
}


/**
  \brief   Reverse byte order (32 bit)
  \details Reverses the byte order in integer value.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
[[gnu::always_inline]] static inline uint32_t __REV(uint32_t value)
{
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
  return __builtin_bswap32(value);
#else
  uint32_t result;

  asm volatile ("rev %w0, %w1" : __CMSIS_GCC_OUT_REG (result) : __CMSIS_GCC_USE_REG (value) );
  return(result);
#endif
}


/**
  \brief   Reverse byte order (16 bit)
  \details Reverses the byte order in two unsigned short values.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
[[gnu::always_inline]] static inline uint32_t __REV16(uint32_t value)
{
  uint32_t result;

  asm volatile ("rev16 %w0, %w1" : __CMSIS_GCC_OUT_REG (result) : __CMSIS_GCC_USE_REG (value) );
  return(result);
}


/**
  \brief   Reverse byte order in signed short value
  \details Reverses the byte order in a signed short value with sign extension to integer.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
[[gnu::always_inline]] static inline int32_t __REVSH(int32_t value)
{
#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
  return (short)__builtin_bswap16(value);
#else
  int32_t result;

  asm volatile ("revsh %w0, %w1" : __CMSIS_GCC_OUT_REG (result) : __CMSIS_GCC_USE_REG (value) );
  return(result);
#endif
}


/**
  \brief   Rotate Right in unsigned value (32 bit)
  \details Rotate Right (immediate) provides the value of the contents of a register rotated by a variable number of bits.
  \param [in]    value  Value to rotate
  \param [in]    value  Number of Bits to rotate
  \return               Rotated value
 */
[[gnu::always_inline]] static inline uint32_t __ROR(uint32_t op1, uint32_t op2)
{
  return (op1 >> op2) | (op1 << (32U - op2));
}


/**
  \brief   Breakpoint
  \details Causes the processor to enter Debug state.
           Debug tools can use this to investigate system state when the instruction at a particular address is reached.
  \param [in]    value  is ignored by the processor.
                 If required, a debugger can use it to store additional information about the breakpoint.
 */
#define __BKPT(value)                       asm volatile ("bkpt "#value)


/**
  \brief   Reverse bit order of value
  \details Reverses the bit order of the given value.
  \param [in]    value  Value to reverse
  \return               Reversed value
 */
[[gnu::always_inline]] static inline uint32_t __RBIT(uint32_t value)
{
  uint32_t result;

#if       (__CORTEX_M >= 0x03U) || (__CORTEX_SC >= 300U)
   asm volatile ("rbit %w0, %w1" : "=r" (result) : "r" (value) );
#else
  int32_t s = 4 /*sizeof(v)*/ * 8 - 1; /* extra shift needed at end */

  result = value;                      /* r will be reversed bits of v; first get LSB of v */
  for (value >>= 1U; value; value >>= 1U)
  {
    result <<= 1U;
    result |= value & 1U;
    s--;
  }
  result <<= s;                        /* shift when v's highest bits are zero */
#endif
  return(result);
}


/**
  \brief   Count leading zeros
  \details Counts the number of leading zeros of a data value.
  \param [in]  value  Value to count the leading zeros
  \return             number of leading zeros in value
 */
#define __CLZ             __builtin_clz


#if       (__CORTEX_M >= 0x03U) || (__CORTEX_SC >= 300U)

/**
  \brief   LDR Exclusive (8 bit)
  \details Executes a exclusive LDR instruction for 8 bit value.
  \param [in]    ptr  Pointer to data
  \return             value of type uint8_t at (*ptr)
 */
[[gnu::always_inline]] static inline uint8_t __LDREXB(volatile uint8_t *addr)
{
    uint32_t result;

#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
   asm volatile ("ldrexb %w0, %1" : "=r" (result) : "Q" (*addr) );
#else
    /* Prior to GCC 4.8, "Q" will be expanded to [rx, #0] which is not
       accepted by assembler. So has to use following less efficient pattern.
    */
   asm volatile ("ldrexb %w0, [%1]" : "=r" (result) : "r" (addr) : "memory" );
#endif
   return ((uint8_t) result);    /* Add explicit type cast here */
}


/**
  \brief   LDR Exclusive (16 bit)
  \details Executes a exclusive LDR instruction for 16 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint16_t at (*ptr)
 */
[[gnu::always_inline]] static inline uint16_t __LDREXH(volatile uint16_t *addr)
{
    uint32_t result;

#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
   asm volatile ("ldrexh %w0, %1" : "=r" (result) : "Q" (*addr) );
#else
    /* Prior to GCC 4.8, "Q" will be expanded to [rx, #0] which is not
       accepted by assembler. So has to use following less efficient pattern.
    */
   asm volatile ("ldrexh %w0, [%1]" : "=r" (result) : "r" (addr) : "memory" );
#endif
   return ((uint16_t) result);    /* Add explicit type cast here */
}


/**
  \brief   LDR Exclusive (32 bit)
  \details Executes a exclusive LDR instruction for 32 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint32_t at (*ptr)
 */
[[gnu::always_inline]] static inline uint32_t __LDREXW(volatile uint32_t *addr)
{
    uint32_t result;

   asm volatile ("ldrex %w0, %w1" : "=r" (result) : "Q" (*addr) );
   return(result);
}


/**
  \brief   STR Exclusive (8 bit)
  \details Executes a exclusive STR instruction for 8 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
[[gnu::always_inline]] static inline uint32_t __STREXB(uint8_t value, volatile uint8_t *addr)
{
   uint32_t result;

   asm volatile ("strexb %w0, %w2, %w1" : "=&r" (result), "=Q" (*addr) : "r" ((uint32_t)value) );
   return(result);
}


/**
  \brief   STR Exclusive (16 bit)
  \details Executes a exclusive STR instruction for 16 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
[[gnu::always_inline]] static inline uint32_t __STREXH(uint16_t value, volatile uint16_t *addr)
{
   uint32_t result;

   asm volatile ("strexh %w0, %w2, %w1" : "=&r" (result), "=Q" (*addr) : "r" ((uint32_t)value) );
   return(result);
}


/**
  \brief   STR Exclusive (32 bit)
  \details Executes a exclusive STR instruction for 32 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
  \return          0  Function succeeded
  \return          1  Function failed
 */
[[gnu::always_inline]] static inline uint32_t __STREXW(uint32_t value, volatile uint32_t *addr)
{
   uint32_t result;

   asm volatile ("strex %w0, %w2, %w1" : "=&r" (result), "=Q" (*addr) : "r" (value) );
   return(result);
}


/**
  \brief   Remove the exclusive lock
  \details Removes the exclusive lock which is created by LDREX.
 */
[[gnu::always_inline]] static inline void __CLREX(void)
{
  asm volatile ("clrex" ::: "memory");
}


/**
  \brief   Signed Saturate
  \details Saturates a signed value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (1..32)
  \return             Saturated value
 */
#define __SSAT(ARG1,ARG2) \
({                          \
  uint32_t __RES, __ARG1 = (ARG1); \
  asm ("ssat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) ); \
  __RES; \
 })


/**
  \brief   Unsigned Saturate
  \details Saturates an unsigned value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (0..31)
  \return             Saturated value
 */
#define __USAT(ARG1,ARG2) \
({                          \
  uint32_t __RES, __ARG1 = (ARG1); \
  asm ("usat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) ); \
  __RES; \
 })


/**
  \brief   Rotate Right with Extend (32 bit)
  \details Moves each bit of a bitstring right by one bit.
           The carry input is shifted in at the left end of the bitstring.
  \param [in]    value  Value to rotate
  \return               Rotated value
 */
[[gnu::always_inline]] static inline uint32_t __RRX(uint32_t value)
{
  uint32_t result;

  asm volatile ("rrx %w0, %w1" : __CMSIS_GCC_OUT_REG (result) : __CMSIS_GCC_USE_REG (value) );
  return(result);
}


/**
  \brief   LDRT Unprivileged (8 bit)
  \details Executes a Unprivileged LDRT instruction for 8 bit value.
  \param [in]    ptr  Pointer to data
  \return             value of type uint8_t at (*ptr)
 */
[[gnu::always_inline]] static inline uint8_t __LDRBT(volatile uint8_t *addr)
{
    uint32_t result;

#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
   asm volatile ("ldrbt %w0, %1" : "=r" (result) : "Q" (*addr) );
#else
    /* Prior to GCC 4.8, "Q" will be expanded to [rx, #0] which is not
       accepted by assembler. So has to use following less efficient pattern.
    */
   asm volatile ("ldrbt %w0, [%1]" : "=r" (result) : "r" (addr) : "memory" );
#endif
   return ((uint8_t) result);    /* Add explicit type cast here */
}


/**
  \brief   LDRT Unprivileged (16 bit)
  \details Executes a Unprivileged LDRT instruction for 16 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint16_t at (*ptr)
 */
[[gnu::always_inline]] static inline uint16_t __LDRHT(volatile uint16_t *addr)
{
    uint32_t result;

#if (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
   asm volatile ("ldrht %w0, %1" : "=r" (result) : "Q" (*addr) );
#else
    /* Prior to GCC 4.8, "Q" will be expanded to [rx, #0] which is not
       accepted by assembler. So has to use following less efficient pattern.
    */
   asm volatile ("ldrht %w0, [%1]" : "=r" (result) : "r" (addr) : "memory" );
#endif
   return ((uint16_t) result);    /* Add explicit type cast here */
}


/**
  \brief   LDRT Unprivileged (32 bit)
  \details Executes a Unprivileged LDRT instruction for 32 bit values.
  \param [in]    ptr  Pointer to data
  \return        value of type uint32_t at (*ptr)
 */
[[gnu::always_inline]] static inline uint32_t __LDRT(volatile uint32_t *addr)
{
    uint32_t result;

   asm volatile ("ldrt %w0, %w1" : "=r" (result) : "Q" (*addr) );
   return(result);
}


/**
  \brief   STRT Unprivileged (8 bit)
  \details Executes a Unprivileged STRT instruction for 8 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
[[gnu::always_inline]] static inline void __STRBT(uint8_t value, volatile uint8_t *addr)
{
   asm volatile ("strbt %w1, %0" : "=Q" (*addr) : "r" ((uint32_t)value) );
}


/**
  \brief   STRT Unprivileged (16 bit)
  \details Executes a Unprivileged STRT instruction for 16 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
[[gnu::always_inline]] static inline void __STRHT(uint16_t value, volatile uint16_t *addr)
{
   asm volatile ("strht %w1, %0" : "=Q" (*addr) : "r" ((uint32_t)value) );
}


/**
  \brief   STRT Unprivileged (32 bit)
  \details Executes a Unprivileged STRT instruction for 32 bit values.
  \param [in]  value  Value to store
  \param [in]    ptr  Pointer to location
 */
[[gnu::always_inline]] static inline void __STRT(uint32_t value, volatile uint32_t *addr)
{
   asm volatile ("strt %w1, %w0" : "=Q" (*addr) : "r" (value) );
}

#endif /* (__CORTEX_M >= 0x03U) || (__CORTEX_SC >= 300U) */

/*@}*/ /* end of group CMSIS_Core_InstructionInterface */


/* ###################  Compiler specific Intrinsics  ########################### */
/** \defgroup CMSIS_SIMD_intrinsics CMSIS SIMD Intrinsics
  Access to dedicated SIMD instructions
  @{
*/

#if (__CORTEX_M >= 0x04U)  /* only for Cortex-M4 and above */

[[gnu::always_inline]] static inline uint32_t __SADD8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("sadd8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __QADD8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("qadd8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SHADD8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("shadd8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UADD8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uadd8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UQADD8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uqadd8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UHADD8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uhadd8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}


[[gnu::always_inline]] static inline uint32_t __SSUB8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("ssub8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __QSUB8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("qsub8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SHSUB8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("shsub8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __USUB8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("usub8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UQSUB8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uqsub8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UHSUB8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uhsub8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}


[[gnu::always_inline]] static inline uint32_t __SADD16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("sadd16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __QADD16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("qadd16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SHADD16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("shadd16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UADD16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uadd16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UQADD16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uqadd16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UHADD16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uhadd16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SSUB16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("ssub16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __QSUB16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("qsub16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SHSUB16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("shsub16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __USUB16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("usub16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UQSUB16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uqsub16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UHSUB16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uhsub16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SASX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("sasx %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __QASX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("qasx %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SHASX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("shasx %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UASX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uasx %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UQASX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uqasx %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UHASX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uhasx %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SSAX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("ssax %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __QSAX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("qsax %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SHSAX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("shsax %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __USAX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("usax %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UQSAX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uqsax %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UHSAX(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uhsax %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __USAD8(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("usad8 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __USADA8(uint32_t op1, uint32_t op2, uint32_t op3)
{
  uint32_t result;

  asm volatile ("usada8 %w0, %w1, %w2, %w3" : "=r" (result) : "r" (op1), "r" (op2), "r" (op3) );
  return(result);
}

#define __SSAT16(ARG1,ARG2) \
({                          \
  int32_t __RES, __ARG1 = (ARG1); \
  asm ("ssat16 %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) ); \
  __RES; \
 })

#define __USAT16(ARG1,ARG2) \
({                          \
  uint32_t __RES, __ARG1 = (ARG1); \
  asm ("usat16 %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1) ); \
  __RES; \
 })

[[gnu::always_inline]] static inline uint32_t __UXTB16(uint32_t op1)
{
  uint32_t result;

  asm volatile ("uxtb16 %w0, %w1" : "=r" (result) : "r" (op1));
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __UXTAB16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("uxtab16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SXTB16(uint32_t op1)
{
  uint32_t result;

  asm volatile ("sxtb16 %w0, %w1" : "=r" (result) : "r" (op1));
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SXTAB16(uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("sxtab16 %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SMUAD  (uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("smuad %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SMUADX (uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("smuadx %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SMLAD (uint32_t op1, uint32_t op2, uint32_t op3)
{
  uint32_t result;

  asm volatile ("smlad %w0, %w1, %w2, %w3" : "=r" (result) : "r" (op1), "r" (op2), "r" (op3) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SMLADX (uint32_t op1, uint32_t op2, uint32_t op3)
{
  uint32_t result;

  asm volatile ("smladx %w0, %w1, %w2, %w3" : "=r" (result) : "r" (op1), "r" (op2), "r" (op3) );
  return(result);
}

[[gnu::always_inline]] static inline uint64_t __SMLALD (uint32_t op1, uint32_t op2, uint64_t acc)
{
  union llreg_u{
    uint32_t w32[2];
    uint64_t w64;
  } llr;
  llr.w64 = acc;

#ifndef __ARMEB__   /* Little endian */
  asm volatile ("smlald %w0, %w1, %w2, %w3" : "=r" (llr.w32[0]), "=r" (llr.w32[1]): "r" (op1), "r" (op2) , "0" (llr.w32[0]), "1" (llr.w32[1]) );
#else               /* Big endian */
  asm volatile ("smlald %w0, %w1, %w2, %w3" : "=r" (llr.w32[1]), "=r" (llr.w32[0]): "r" (op1), "r" (op2) , "0" (llr.w32[1]), "1" (llr.w32[0]) );
#endif

  return(llr.w64);
}

[[gnu::always_inline]] static inline uint64_t __SMLALDX (uint32_t op1, uint32_t op2, uint64_t acc)
{
  union llreg_u{
    uint32_t w32[2];
    uint64_t w64;
  } llr;
  llr.w64 = acc;

#ifndef __ARMEB__   /* Little endian */
  asm volatile ("smlaldx %w0, %w1, %w2, %w3" : "=r" (llr.w32[0]), "=r" (llr.w32[1]): "r" (op1), "r" (op2) , "0" (llr.w32[0]), "1" (llr.w32[1]) );
#else               /* Big endian */
  asm volatile ("smlaldx %w0, %w1, %w2, %w3" : "=r" (llr.w32[1]), "=r" (llr.w32[0]): "r" (op1), "r" (op2) , "0" (llr.w32[1]), "1" (llr.w32[0]) );
#endif

  return(llr.w64);
}

[[gnu::always_inline]] static inline uint32_t __SMUSD  (uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("smusd %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SMUSDX (uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("smusdx %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SMLSD (uint32_t op1, uint32_t op2, uint32_t op3)
{
  uint32_t result;

  asm volatile ("smlsd %w0, %w1, %w2, %w3" : "=r" (result) : "r" (op1), "r" (op2), "r" (op3) );
  return(result);
}

[[gnu::always_inline]] static inline uint32_t __SMLSDX (uint32_t op1, uint32_t op2, uint32_t op3)
{
  uint32_t result;

  asm volatile ("smlsdx %w0, %w1, %w2, %w3" : "=r" (result) : "r" (op1), "r" (op2), "r" (op3) );
  return(result);
}

[[gnu::always_inline]] static inline uint64_t __SMLSLD (uint32_t op1, uint32_t op2, uint64_t acc)
{
  union llreg_u{
    uint32_t w32[2];
    uint64_t w64;
  } llr;
  llr.w64 = acc;

#ifndef __ARMEB__   /* Little endian */
  asm volatile ("smlsld %w0, %w1, %w2, %w3" : "=r" (llr.w32[0]), "=r" (llr.w32[1]): "r" (op1), "r" (op2) , "0" (llr.w32[0]), "1" (llr.w32[1]) );
#else               /* Big endian */
  asm volatile ("smlsld %w0, %w1, %w2, %w3" : "=r" (llr.w32[1]), "=r" (llr.w32[0]): "r" (op1), "r" (op2) , "0" (llr.w32[1]), "1" (llr.w32[0]) );
#endif

  return(llr.w64);
}

[[gnu::always_inline]] static inline uint64_t __SMLSLDX (uint32_t op1, uint32_t op2, uint64_t acc)
{
  union llreg_u{
    uint32_t w32[2];
    uint64_t w64;
  } llr;
  llr.w64 = acc;

#ifndef __ARMEB__   /* Little endian */
  asm volatile ("smlsldx %w0, %w1, %w2, %w3" : "=r" (llr.w32[0]), "=r" (llr.w32[1]): "r" (op1), "r" (op2) , "0" (llr.w32[0]), "1" (llr.w32[1]) );
#else               /* Big endian */
  asm volatile ("smlsldx %w0, %w1, %w2, %w3" : "=r" (llr.w32[1]), "=r" (llr.w32[0]): "r" (op1), "r" (op2) , "0" (llr.w32[1]), "1" (llr.w32[0]) );
#endif

  return(llr.w64);
}

[[gnu::always_inline]] static inline uint32_t __SEL  (uint32_t op1, uint32_t op2)
{
  uint32_t result;

  asm volatile ("sel %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline  int32_t __QADD( int32_t op1,  int32_t op2)
{
  int32_t result;

  asm volatile ("qadd %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

[[gnu::always_inline]] static inline  int32_t __QSUB( int32_t op1,  int32_t op2)
{
  int32_t result;

  asm volatile ("qsub %w0, %w1, %w2" : "=r" (result) : "r" (op1), "r" (op2) );
  return(result);
}

#define __PKHBT(ARG1,ARG2,ARG3) \
({                          \
  uint32_t __RES, __ARG1 = (ARG1), __ARG2 = (ARG2); \
  asm ("pkhbt %0, %1, %2, lsl %3" : "=r" (__RES) :  "r" (__ARG1), "r" (__ARG2), "I" (ARG3)  ); \
  __RES; \
 })

#define __PKHTB(ARG1,ARG2,ARG3) \
({                          \
  uint32_t __RES, __ARG1 = (ARG1), __ARG2 = (ARG2); \
  if (ARG3 == 0) \
    asm ("pkhtb %0, %1, %2" : "=r" (__RES) :  "r" (__ARG1), "r" (__ARG2)  ); \
  else \
    asm ("pkhtb %0, %1, %2, asr %3" : "=r" (__RES) :  "r" (__ARG1), "r" (__ARG2), "I" (ARG3)  ); \
  __RES; \
 })

[[gnu::always_inline]] static inline uint32_t __SMMLA (int32_t op1, int32_t op2, int32_t op3)
{
 int32_t result;

 asm volatile ("smmla %w0, %w1, %w2, %w3" : "=r" (result): "r"  (op1), "r" (op2), "r" (op3) );
 return(result);
}

#endif /* (__CORTEX_M >= 0x04) */
/*@} end of group CMSIS_SIMD_intrinsics */


#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

#endif /* __CMSIS_GCC_H */
