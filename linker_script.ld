/* This is the linker script for an ARM cortex-m4 processor, it's taken mostly from: 
https://github.com/STMicroelectronics/STM32CubeF4/blob/master/Projects/STM32F429ZI-Nucleo/Templates/STM32CubeIDE/STM32F429ZITX_FLASH.ld
*/

/* ENTRY POINT */
ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Define stack and heap size */
_Min_Heap_Size = 0x200;
_Min_Stack_Size = 0x400;

/* Define the memory regions */
MEMORY {
  ROM     (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  CCMRAM  (xrw) : ORIGIN = 0x10000000, LENGTH = 64K
  RAM     (xrw) : ORIGIN = 0x20000000, LENGTH = 192K
}

SECTIONS {
  
  /* VECTOR TABLE */
  .isr_vector (READONLY):
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >ROM

  /* TEXT SECTION */
  .text (READONLY):
  {
    . = ALIGN(4);
    *(.text)          /* .text section of code */
    *(.text*)         /* .text* section of code */
    *(.glue_7)        /* glue arm to thumb code, ARM M4 only has thumb instructions but include anyway, empty is benign */
    *(.glue_7t)       /* glue thumb to arm code ARM M4 only has thumb instructions but include anyway, empty is benign */
    *(.eh_frame)

    KEEP (*(.init))   /* Code for before main */
    KEEP (*(.fini))   /* Code for after main */

    . = ALIGN(4);
    _etext = .;       /* End of text section GLOBAL */
  } >ROM

  /* READONLY DATA SECTION */
  .rodata (READONLY):
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >ROM

  /* EXCEPTION TABLE, use for exception handling & stack unwinding */
  .ARM.extab (READONLY) : /* READONLY only support with gcc above 11 */
  {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >ROM

  /* PREINITIALIZATION SECTION, for stuff before regular init */
  .preinit_array (READONLY) : 
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >ROM

  /* INIT ARRAY SECTION, contains pointers to function that have to be executed at start up */
  .init_array(READONLY) :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >ROM

  /* FINI ARRAY SECTION, contains pointers to functions have that to be executed on termination */
  .fini_array (READONLY) :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >ROM

  /* Data section start address, _sidata is the load address that we copy from into ram */
  _sidata = LOADADDR(.data);

  /* DATA SECTION */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;     /* GLOBAL symbol start */
    *(.data)
    *(.data.)
    *(.RamFunc) 
    *(.RamFunc*)

    . = ALIGN(4);
    _edata = .;     /* GLOBAL symbol end */
  } >RAM AT> ROM

  _siccmram = LOADADDR(.ccmram);

  /* CCRAM SECTION */
  .ccmram :
  {
    . = ALIGN(4);
    _sccmram = .; /* GLOBAL symbol start */ 
    *(.ccmram)
    *(.ccmram)

    . = ALIGN(4);
    _eccmram = .; /* GLOBAL symbol end */
  } >CCMRAM AT> ROM

  . = ALIGN(4);

  /* BSS SECTION */
  .bss :
  {
    _sbss = .;    /* GLOBAL symbol start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;    /* GLOBAL symbol end */
    __bss_end__ = _ebss;
  } >RAM

  /* USER HEAP */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM

  /* DISCARD gets rid of sections not included in the script from these libs, also maybe removes these from the final elf? */
  /DISCARD/ :
  {
    libc.a (*)
    libm.a (*)
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes)}
}