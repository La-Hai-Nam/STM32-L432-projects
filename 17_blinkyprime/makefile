USER=mrmr
.PHONY: all
all: blinkyprime.bin

blinkyprime.bin: blinkyprime.elf
	arm-none-eabi-objcopy -O binary -S blinkyprime.elf blinkyprime.bin

blinkyprime.elf: head.o usart.o blinkyprime.o
	arm-none-eabi-gcc $^ -mcpu=cortex-m4 -mthumb -nostartfiles -Wl,-Ttext=0x8000000 -lm -o blinkyprime.elf

head.o: head.S
	arm-none-eabi-gcc $< -mcpu=cortex-m4 -mthumb -c -o $@

usart.o: usart.c
	arm-none-eabi-gcc $< -mcpu=cortex-m4 -mthumb -c -o $@

blinkyprime.o: blinkyprime.c usart.h
	arm-none-eabi-gcc $< -mcpu=cortex-m4 -mthumb -c -o $@

.PHONY: clean
clean:
	rm -f head.o usart.o blinkyprime.o blinkyprime.bin blinkyprime.elf

.PHONY:openocd
openocd:
	openocd -f nucleo-l4.cfg

.PHONY:gdb
gdb:blinkyprime.elf
	gdb-multiarch blinkyprime.elf -ex "target remote localhost:3333" -ex "monitor reset init" -ex "b _start"


.PHONY: upload
upload: blinkyprime.bin
	cp blinkyprime.bin /media/$(USER)/NODE_L432KC

