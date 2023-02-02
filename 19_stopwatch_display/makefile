USER=mrmr
.PHONY: all
all: stopwatch_display.bin

stopwatch_display.bin: stopwatch_display.elf
	arm-none-eabi-objcopy -g -O binary -S stopwatch_display.elf stopwatch_display.bin

stopwatch_display.elf: head.o utility.o tm1637.o stopwatch_display.o
<<<<<<< HEAD
	arm-none-eabi-gcc $^ -g -mcpu=cortex-m4 -mthumb -nostartfiles -Wl,-Ttext=0x8000000 -Wl,-Tbss=0x20000000 -lm -o stopwatch_display.elf
=======
	arm-none-eabi-gcc $^ -g -mcpu=cortex-m4 -mthumb -nostartfiles -Wl,-Ttext=0x8000000 -Wl,-Tbss=0x20000000  -lm -o stopwatch_display.elf
>>>>>>> c8e76fa00c4365a3eb68492fbd2e19f39e78bb6c

head.o: head.S
	arm-none-eabi-gcc $< -g -mcpu=cortex-m4 -mthumb -c -o $@

utility.o: utility.c utility.h tm1637.h
	arm-none-eabi-gcc $< -g -mcpu=cortex-m4 -mthumb -c -o $@
	
tm1637.o: tm1637.c utility.h
	arm-none-eabi-gcc $< -g -mcpu=cortex-m4 -mthumb -c -o $@

stopwatch_display.o: stopwatch_display.c tm1637.h utility.h
	arm-none-eabi-gcc $< -g -mcpu=cortex-m4 -mthumb -c -o $@

.PHONY: clean
clean:
	rm -f head.o utility.o tm1637.o stopwatch_display.o stopwatch_display.bin stopwatch_display.elf

.PHONY:openocd
openocd:
	openocd -f nucleo-l4.cfg

.PHONY:gdb
gdb:stopwatch_display.elf
	gdb-multiarch stopwatch_display.elf -ex "target remote localhost:3333" -ex "monitor reset init" -ex "b _start"


.PHONY: upload
upload: stopwatch_display.bin
	cp stopwatch_display.bin /media/$(USER)/NODE_L432KC

