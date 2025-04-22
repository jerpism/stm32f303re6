C_SOURCES = $(wildcard src/*.c)
ASM_SOURCES = $(wildcard src/*.s)
HEADERS = $(wildcard *.h)
OBJ = ${C_SOURCES:.c=.o} ${ASM_SOURCES:.s=.o}

CCFLAGS = -mcpu=cortex-m4 -Og -ggdb \
          -ffreestanding -nostdlib -nostdinc -nodefaultlibs \
	  -Wall -Wextra \
	  -I include/

all: os.bin

flash: all
	st-flash --reset write os.bin 0x8000000

os.bin : os.elf 
	arm-none-eabi-objcopy -O binary $^ $@

os.elf : ${OBJ}
	arm-none-eabi-ld -T link.ld $^ -o $@


%.o : %.c ${HEADERS}
	arm-none-eabi-gcc $(CCFLAGS) -c $< -o $@ 
%.o : %.s 
	arm-none-eabi-gcc $(CCFLAGS) -c $< -o $@

clean:
	rm -rf *.bin *.o *.elf
	rm -rf src/*.o
