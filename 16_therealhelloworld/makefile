USER=mrmr
.PHONY: all
all: therealhelloworld.bin

therealhelloworld.bin: therealhelloworld.elf
	arm-none-eabi-objcopy -O binary -S therealhelloworld.elf therealhelloworld.bin

therealhelloworld.elf: head.o usart.o therealhelloworld.o
	arm-none-eabi-gcc $^ -mcpu=cortex-m4 -mthumb -nostartfiles -Wl,-Ttext=0x8000000  -lm -o therealhelloworld.elf

head.o: head.S
	arm-none-eabi-gcc $< -mcpu=cortex-m4 -mthumb -c -o $@

usart.o: usart.c
	arm-none-eabi-gcc $< -mcpu=cortex-m4 -mthumb -c -o $@

therealhelloworld.o: therealhelloworld.c usart.h
	arm-none-eabi-gcc $< -mcpu=cortex-m4 -mthumb -c -o $@

.PHONY: clean
clean:
	rm -f head.o usart.o therealhelloworld.o therealhelloworld.bin therealhelloworld.elf

.PHONY:openocd
openocd:
	openocd -f nucleo-l4.cfg

.PHONY:gdb
gdb:therealhelloworld.elf
	gdb-multiarch therealhelloworld.elf -ex "target remote localhost:3333" -ex "monitor reset init" -ex "b _start"


.PHONY: upload
upload: therealhelloworld.bin
	cp therealhelloworld.bin /media/$(USER)/NODE_L432KC

