USER=mrmr
.PHONY: all
all: stopwatch.bin

stopwatch.bin: stopwatch.elf
	arm-none-eabi-objcopy -g -O binary -S stopwatch.elf stopwatch.bin

stopwatch.elf: head.o utility.o stopwatch.o
	arm-none-eabi-gcc $^ -g -mcpu=cortex-m4 -mthumb -nostartfiles -Wl,-Ttext=0x8000000 -Wl,-Tbss=0x20000000  -lm -o stopwatch.elf

head.o: head.S
	arm-none-eabi-gcc $< -g -mcpu=cortex-m4 -mthumb -c -o $@

utility.o: utility.c
	arm-none-eabi-gcc $< -g -mcpu=cortex-m4 -mthumb -c -o $@

stopwatch.o: stopwatch.c 
	arm-none-eabi-gcc $< -g -mcpu=cortex-m4 -mthumb -c -o $@

.PHONY: clean
clean:
	rm -f head.o utility.o stopwatch.o stopwatch.bin stopwatch.elf

.PHONY:openocd
openocd:
	openocd -f nucleo-l4.cfg

.PHONY:gdb
gdb:stopwatch.elf
	gdb-multiarch stopwatch.elf -ex "target remote localhost:3333" -ex "monitor reset init" -ex "b _start"


.PHONY: upload
upload: stopwatch.bin
	cp stopwatch.bin /media/$(USER)/NODE_L432KC

