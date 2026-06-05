BOOT SEQUENCE:

Reset can be done w/ HW (With a button) or with SW. After reset:
1. All registers are set to their reset values. 
2. Processor determines the boot mode, based on the boot pins. This determines where to boot from, either FLASH, SRAM, or External Memory. 
3. Main Stack Pointer (MSP) is fetched from address 0x0 and loaded into the MSP register.
4. Program Counter is fetched from address 0x4 and loaded into the PC register.
value of PC, is the address of the Reset Handler function/subroutine. This is the entry point of the program. 
5. Link register is loaded with 0xFFFFFFFF.
