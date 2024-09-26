#include <cstddef>
#include <cstdint>
#include <cerrno>

/* 
This function is taken from here: https://github.com/STMicroelectronics/STM32CubeF4/blob/master/Projects/STM32F429ZI-Nucleo/Examples/BSP/STM32CubeIDE/Example/User/sysmem.c

 * ############################################################################
 * #  .data  #  .bss  #       newlib heap       #          MSP stack          #
 * #         #        #                         # Reserved by _Min_Stack_Size #
 * ############################################################################
 * ^-- RAM start      ^-- _end                             _estack, RAM end --^

dynamic memory is allocated starting from the end of the _end linker symbol

*/

#ifdef __cplusplus
extern "C" {
#endif


static std::uint8_t *__sbrk_heap_end = nullptr;

void* _sbrk(std::ptrdiff_t incr) {
  extern std::uint8_t _end; /* Symbol defined in the linker script */
  extern std::uint8_t _estack; /* Symbol defined in the linker script */
  extern std::uint32_t _Min_Stack_Size; /* Symbol defined in the linker script */
  const std::uint32_t stack_limit = (uint32_t)&_estack - (uint32_t)&_Min_Stack_Size;
  const std::uint8_t *max_heap = (uint8_t *)stack_limit;
  std::uint8_t *prev_heap_end;

  /* Initialize heap end at first call */
  if (nullptr == __sbrk_heap_end)
  {
    __sbrk_heap_end = &_end;
  }

  /* Protect heap from growing into the reserved MSP stack */
  if (__sbrk_heap_end + incr > max_heap)
  {
    errno = ENOMEM;
    return (void *)-1;
  }

  prev_heap_end = __sbrk_heap_end;
  __sbrk_heap_end += incr;

  return (void *)prev_heap_end;
}


#ifdef __cplusplus
}
#endif
