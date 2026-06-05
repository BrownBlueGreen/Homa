
#include "Drivers/pin.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#define QUANTA 10
typedef uint32_t TaskProfiler;

TaskProfiler Task0_Profiler, Task1_Profiler, Task2_Profiler;

int main(){

  osKernelInit();
  osKernelAddThreads(&task0, &task1, &task2);
  osKernelLaunch(QUANTA);

  while(1) {}
  return 0;
}

#ifdef __cplusplus
}
#endif