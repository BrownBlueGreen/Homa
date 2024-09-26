CC=arm-none-eabi-g++
MACH=cortex-m4
INST = -mthumb
DIAL = c++20
CFLAGS = -mfloat-abi=hard -fno-exceptions -mcpu=$(MACH) $(INST) -std=$(DIAL) -Wall -c
LDFLAGS = -mfloat-abi=hard -mcpu=$(MACH) $(INST) --specs=nano.specs -T linker_script.ld -Wl,-Map=final.map

# target: dependency
# \tab receipt
# some notes:
# $^ --> This means replace with depency and $@ means target
all:main.o startup.o syscalls.o sysmem.o final.elf

main.o : main.cpp
		$(CC) $(CFLAGS) $^ -o $@

startup.o : startup.cpp
		$(CC) $(CFLAGS) $^ -o $@

syscalls.o : syscalls.cpp
		$(CC) $(CFLAGS) $^ -o $@

sysmem.o : sysmem.cpp
		$(CC) $(CFLAGS) $^ -o $@

final.elf: main.o startup.o syscalls.o sysmem.o
		$(CC) $(LDFLAGS) $^ -o $@

clean:
		rm -rf *.o *.elf *.map

load:
	openocd -f board/stm32f429discovery.cfg