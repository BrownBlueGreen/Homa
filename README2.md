Some of the includes can be confusing but the general depency outline is this:

macros.h --> gcc_funcs.h --> core_mmap.h --> memory_map.h --> system.h --> Several files (eg. sysinit.cpp)

**macros.h** --> This file contains a bunch of macros that are used through out the whole project. Normally (at least in the ST libs that I've looked at) these macros are spread out across a few diff files. This is kind of annoying and makes it hard to follow so I moved them all here so the depenencies are a bit cleaner. 

**gcc_funcs.h** --> This is a slightly altered version of cmsis_gcc.h. It defines a bunch of functions for setting and getting register and such. And becuase these functions are done in assembly language in C (using the asm keyword), they are compiler specific and thus included here. 

**core.h** --> This file contains a bunch of core ARM specific registers that exist regardless of the manufacturers. For example regardless of whether the uC is from ST or TI these registers are present. 

**memory_map.h** --> This file contains the ST ARM specific registers. These are specific to ST. 

**system.h** --> This file just has some macro definitions for now. 

