AUTHOR: Babak Khorsand
DATE:   09/23/2024

PROJECT HOMA VERSION 0.1

This project is for a real time operating system for ARM microcontrollers. 

Starting with ARM F429 and potentially moving onto other platforms. 

The project is an exercise in system design and software engineering. It's a learning tool. And the goal is to develop an operating system in C++, first as an RTOS and then maybe onto a more general operating system for something like an mp157.

Why C++? Becuase I think it would be cool to have tasks as classes and to encapsulate peripherals in classes, kind of like arduino. There isn't a need per se, just for fun. 

gpio_a LED_PIN(some channel);
LED_PIN.enable()
LED_PIN.disable();

Kind of like arduino

# SOME FOUNDATIONAL STUFF
If you have some source and header files, and want to turn it into something that does stuff (an executable) you have to compile it. For microcontrollers and single board computers, you have to use a cross compilation tool chain. This means that you compile the program on your local (or remote) machine and the download it to the microcontroller. This is because the uC doesn't have a compiler on it (single board computer might have a compiler).

The toolchain generally comes with binaries to compile, assembly, link, analyze, and debug applications. 

For ARM, 2 toolchains available: GCC and ARMCC. GCC is free so going with that, ARMCC is partly free but for everything you have to pay for a license. The name of the actual toolchain I'm using is GNU-ARM-TOOLCHAIN from here: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads, which contains the gnu-arm-embedded-toolchain plus some other stuff. 

Let go over the basics of the toolchain:

# COMPILER, ASSEMBLER, LINKER:
arm-none-eabi-gcc (for C)
arm-none-eabi-g++ (for C/C++)

# ASSEMBLER
arm-none-eabi-as

# LINKER
arm-none-eabi-ld

# ELF FILE ANALYZER
arm-none-eabi-objdump
arm-none-eabi-readelf
arm-none-eabi-nm

# FORMAT CONVERTER
arm-none-eabi-objcopy

There's a bunch of other tools and libraries and such that I'm ignoring. 

# BUILD PROCESS:
When you call the compiler it goes through the following stages:

1. Preprocessing: 
Here all preprocessor directives will be resolved, like #include. The output file with ending .i will be created. So main.c --> main.i

**** Between 1. and 2. Parsing is also done where there's C syntax checks.

2. Code generation stage:
Here source file like main.i will be converted to assembly language. High level code statements are converted to processor architectural mnemonics. This is thumb or AArch32/64 instructions. This produces a file with .s ending. So main.i --> main.s

3. Assembler stage:
Here processor mnemonics are converted to opcodes, this is machine code. This outputs a file with .o ending called a relocatable object file. So main.s --> main.o

main.o is architecture specific machine code with no absolute addresses, hence why it's called "relocatable". You can't see the .i and .s files  unless you specifically request it. 

4. Linking stage:
Here all the .o files are taken by the linker, which resolves all the symbols and merges the obect files into a single executable. The output of this stage is a file with ending .elf. So *.o --> main.elf. elf stands for executable and linkable format. elf files can the be converted to other formats like .bin

And that's it! The general process goes: preprocess --> compilation --> linking. And just calling arm-none-eabi-gcc/g++ does all of this. But if you want you can do each indivitually or ask the program to save the inbetween files. 

Example for just building main file

arm-none-eabi-gcc main.c -o main.o

This instructs the compiler to take and input file: main.c

Specifies option to generate an output: -o

Specifies name of output file: main.o

Default behavior of this command is to compile assemble and link. If you want to just compile and assemble, no linking, provide the argument -c. like so: 

arm-none-eabi-gcc -c main.c -o main.o. 

Commands like -c and -o need to come before the file they apply to. You can browse through the compiler flags and option and general info here: https://gcc.gnu.org/onlinedocs/gcc/.

Another important option to set when involking the compiler is the specific architecture you want to compile for and the optoins are either "march" or mcpu. If you don't you'll get an assembler error, becuase it won't know what the assembly code means. So the command would look something like:

arm-none-eabi-gcc -march=arm7ve main.c -o main.o 

OR

arm-none-eabi-gcc -mcpu=cortex-m4 main.c -o main.o 

Lastly you have to specify the instruction set. For ARM chips, they support either ARM instructions or Thumb instructions, and the default it ARM. Since this project is based on cortex-m, we have to use thumb instructions. Some processors support both and you can switch between them, but not for cortex-m4. The compiler option is -mthumb. So the final command is:

arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb main.c -o main.o 

To only make an assembly file and stop the process before involking the assembler, use -S instead of -c. 
                    
# MAKEFILE
You can automate all the previous steps using a makefile. 

Some quick notes about the make file, the general layout is like this:

-> Some variable definitions for things like compiler flags, the complication command etc.

-> Target you want to build : File target depends on so this would be like ---> main.o : main.cpp
-> A tab, then the recipe of the build. So this would be the variables such that they spell out the command above. 

In the recipe, instead of writing main.o and main.c like: arm-none-eabi-gcc -mcpu=cortex-m4 main.c -o main.o, you can write:

arm-none-eabi-gcc -mcpu=cortex-m4 $^ -o $@

Here $^ means the target mentioned above, and $@ is the dependencies. 


# SOME ANALYZING TOOLS
There's a lot of tools in the toolchain you can use to analyze what you're doing. For example you can use the objdump binary with the -h flag to look at the makeup of a .o file. Doing this will show the different sections of the C code like .text, .data, etc..

When the source files are compiled into object files, the different sections of code don't actually have any addresses. As mentioned earlier that's why it's call relocatable, the compiler doesn't know where these are going in memory, that's the job of the linker script. In the linker script, the address of every section is specified. 

# STARTUP FILE
The startup file contains code that runs before main is called, and it's responsible for setting up the environment in which user code can be written and executed. Some parts of it is target dependent, such as the vector table which depends on the hardware. 

The startup code is name responsible for:

- Placing vector table at the correct address
- Copying appropriate data from FLASH/ROM into RAM. Meaning it has to initialize the .data and .bss sections.
- Call any initialization functions
- Call main function

For ARM this can be written in C, for other platforms it might have to be written in assembly. Since this project is in C++ and we're using g++, the startup file will have to be marked with extern "C". Why? Because, I don't know how the c++ compiler mangles names and so the function symbol should be the same as what's written in the file. This is also true for the main function, make sure that in main.cpp, the main function is also extern C.

In the startup file we also define the isr_vector. This is a vector that holds the addresses of the interupt handlers, like so:

handler1 (declaration)
handler2, 
etc.

vector[] {
  handler1,
  handler2,
  etc.
}

So how are these defined? Well there's 90-something handlers (for cortext-m4), are we going to define them all? No, that would be a lot, so to get around this we give these handlers a weak alias to a default handler. This means that until the user defines a specific handler function for these interrupts, the default handler gets called. 

How do we give a weak alias? By using compiler attributes, so for example, to give a weak alias and tell the compiler it won't throw and exception we do the following:

void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler"), nothrow));

Using attributes is also how we tell the compiler to place the isr vector in the correct section of the code. By default, the array would be considered some initialized data and would be placed in the .data section. But it needs to be at the very beginning of memory so we say:

std::uint32_t vectors[] __attribute__((section(".isr_vector"))) = {
  stuff...
}

Note that the vector array elements need to be cast to std::uint32_t becuase they represent an address, and would otherwise actually be a function pointer.

# LINKER SCRIPT 
So where is the location of .isr_vector and other sections specified? In the linker script. As mentioned earlier, the linker script is used to specificy the addresses of different parts of the program, such as .text, .data, etc... 

The linker refers to the linker script to determine absolute location of sections of the object files. And are wwritten in GNU linker command language, with file extension .ld.

Some linker specific commands:

ENTRY(__SYMBOL_NAME__) --> This command sets the entry point address in the final elf file. Meaning execution starts at the address specified by this command. Here, it's Reset_Handler. The debugger also uses this information to find the first function to execute. 

MEMORY{
  name(attr):ORIGIN = origin, LENGTH = len
} 
--> This command lets you describe the different memory locations and their addresses, such as RAM, ROM, etc... attr is the attributes of this memory region and there's a lot of options. 

The linker uses info from this command to assign sections to memory locations and helps it calculate total code and data memory consumed and does error checking if using too much memory. Typically a linker script uses this command once. 

SECTION{
  .section1, 
  .section2, 
  etc.
} REGION>

--> This command specifies the memory sections of the your program and in which region of memory to place them (memory regions are the ones created with the MEMORY command). This command tells the linker where specifically what addressese to use. It also controls the order of sections in the final elf file. 

The sections mentioned in the linker file I'm using is:

.isr_vector
.text/.text*
.rodata/.rodata*
.ARM.extab -> This is exception table for exception handling and stack unwinding
.preinit_array
.init_array
.fini_array
.ccmram
.bss
.user_heap_stack

When you have multiple object files, each have their own sections described above, and the linker merges them together. 

Also symbols have been mentioned a couple times. In the context of the linker, a symbol is the name of an address. In C symbols have the same name as the variables and functions and what have you, but in C++ the compiler mangles the symbol names. It does this to support overloading and polymosphism and stuff like that. The object files, have a symbol table which shows the addresses of whatever variables and functions that are being used. 


When a final.elf file is created, you have to analyze it to understand resource allocation and to make sure sections are placed at the correct addresses. We can do this using gnu tools. You can also tell the linker to create a map file, this will let us take a peek under the hood. 

To make a map file we give the linker the argument: -Map=filename.map, but sometimes when involking a command like arm-none-eabi-g++ it won't recognize linker specific commands, so you specify by adding "-Wl,". So the full command would be: -Wl,-Map=name.map

Right now the makefile goes like this:

Compile all the .c files into .o

Call the linker to link all the .o files into an .elf

# DEBUGGING

When wanting to donwload the executable or debug our taget, we need to use a debug adaptor. Debug adaptor converts the host protocol to the target protocol, so that these two machines can talk to each other. For example usb <--> SWD/JTAG, and on the host you have to run an application like openOCD.

OpenOCD --> open on chip debugger. It's free and ope  source and lets you debug your applcations using GDB. And GBD works with ARM machines. So the whole picture looks something like this:

User <--> GDB Client <--> OpenOCD <--> ST-LINK driver <--> debug adapter <--> Target board. 

target (SWDCLK) --> | Debug port | --> Processor --> |     AHB AP (Access point) --> Core                 
       (SWD IO) --> |            |                   |                |
                                                     |                V
                                                     |        AHB Interconnect
                                                     |                |
                                                     |                | (AHB BUS)
                                                     |                |
                                                     |                V  
                                                     |   SRAM <-- FLASH controller --> Other peripherals
                                                     |                |
                                                     |                V
                                                     |            FLASH memory


When you have open ocd running listening for a connection, you can run arm-none-eabi-gdb.

Once gdb is open, you have to give it commands. To connect to openOCD, use:

target remote localhost:3333

Once the conection is successful, you can issue commands such as:

1. monitor reset init --> Use "monitor" to specify between gdb and openocd commands
2. monitor flash write image erase final.elf --> This write the image to flash memory and erases the old image
3. monitor reset halt --> This reset uC but does execute, it halts from the beginning
4. monitor resume --> Resumes from halt
5. monitor halt or monitor reset --> to only halt or only reset

Now you can issue any number of command like "read a word at X address", or set a break point, etc..

# USING LIBRARIES 
I will be linking the newlib_nano library for basic support of C/C++ features. For C++ exceptions and RTTI are not supported unless it's specifically enabled. The library that's used depends on the spec file that's passed to the linker. Why do we want a library, well so some hardware independent parts of standard library that depend on low level system call. NOTE, YOU HAVE TO PROVIDE THOSE SYSTEM CALLS. Otherwise the standard lib functions don't do anything. These are system calls for stuff like devices, files, etc..

For example, if you call printf in your code, that library function that function does a bunch of formatting and then calls _write(). Then _write will output to some port like UART or something else. These a bunch of syscalls you'll have to implement. 

Implementation for this was taken from ST github account for stm32f429. the link is in one of the files. 

Ok here the basics of baremetal startup and getting the exectution to main is complete. The next things to do to define system_init and some basic hardware files. 

These should include a bunch of macros and copmiler stuff that I won't do by hand it's too much work, plus it has specifics i don't know about. 