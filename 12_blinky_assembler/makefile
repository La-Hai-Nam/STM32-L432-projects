USER = mrmr
.PHONY: all
all: blinky.bin

blinky.bin: blinky.elf
	arm-none-eabi-objcopy -O binary -S blinky.elf blinky.bin

blinky.elf: blinky.o
	arm-none-eabi-gcc -g blinky.o -nostartfiles -Wl,-Ttext=0x8000000 -o blinky.elf

blinky.o: blinky.S
	arm-none-eabi-gcc -g blinky.S -mcpu=cortex-m4 -mthumb -c -o blinky.o

.PHONY: clean
clean:
	rm -f blinky.o blinky.elf blinky.bin

.PHONY: upload
upload: blinky.bin
	cp blinky.bin /media/$(USER)/NODE_L432KC
