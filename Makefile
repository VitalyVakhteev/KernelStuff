CP := cp
RM := rm -rf
MKDIR := mkdir -pv

BIN = kernel_output_files/kernel
CFG = kernel_code/grub.cfg
ISO_PATH := iso
BOOT_PATH := $(ISO_PATH)/boot
GRUB_PATH := $(BOOT_PATH)/grub

.PHONY: all clean

all: iso
	@echo Make has completed.

bootloader: kernel_code/boot.asm
	nasm -f elf32 kernel_code/boot.asm -o kernel_output_files/boot.o

kernel: kernel_code/kernel.c
	gcc -m32 -c kernel_code/kernel.c -o kernel_output_files/kernel.o

linker: kernel_code/linker.ld bootloader kernel
	ld -m elf_i386 -T kernel_code/linker.ld -o $(BIN) kernel_output_files/boot.o kernel_output_files/kernel.o

iso: linker
	$(MKDIR) $(GRUB_PATH)
	$(CP) $(BIN) $(BOOT_PATH)/kernel
	$(CP) $(CFG) $(GRUB_PATH)
	grub-file --is-x86-multiboot $(BOOT_PATH)/kernel
	grub-mkrescue -o my-kernel.iso $(ISO_PATH)

clean:
	$(RM) kernel_output_files/*.o $(BIN) *.iso $(ISO_PATH)
