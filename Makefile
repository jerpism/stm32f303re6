C_SOURCES = $(wildcard *.c)
ASM_SOURCES = $(wildcard *.s)
HEADERS = $(wildcard *.h)
OBJ = ${C_SOURCES:.c=.o} ${ASM_SOURCES:.s=.o}

CCFLAGS = -nostdlib -mcpu=cortex-m4 -Og -ggdb -I include/

all: os.bin

flash: all
	st-flash --reset write os.bin 0x8000000

os.bin : os.elf 
	arm-none-eabi-objcopy -O binary $^ $@

os.elf : start.o usart.o  ${OBJ}
	arm-none-eabi-ld -T link.ld $^ -o $@


%.o : %.c ${HEADERS}
	arm-none-eabi-gcc $(CCFLAGS) -c $< -o $@ 
%.o : %.s 
	arm-none-eabi-gcc $(CCFLAGS) -c $< -o $@

clean:
	rm -rf *.bin *.o *.elf
