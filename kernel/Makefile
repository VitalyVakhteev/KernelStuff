CROSS_COMPILE =
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AS = nasm

CFLAGS = -m64 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -nostdinc -fno-builtin -Wall -Wextra
LDFLAGS = -T linker.ld -nostdlib

all: kernel.bin

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

boot.o: boot.asm
	$(AS) -f elf64 boot.asm -o boot.o

kernel.bin: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) boot.o kernel.o -o kernel.elf
	objcopy -O binary kernel.elf kernel.bin

run: kernel.bin
	qemu-system-x86_64 -kernel kernel.bin

clean:
	rm -f *.o *.elf *.bin
