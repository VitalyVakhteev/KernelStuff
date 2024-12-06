ARCH      = x86_64
EFIINC    = -I/usr/include/efi -I/usr/include/efi/x86_64
CFLAGS    = -O2 -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -Wall -Wextra $(EFIINC)
LDFLAGS   = -nostdlib -T /usr/lib/elf_x86_64_efi.lds -shared -Bsymbolic -L /usr/lib -lgnuefi -lefi
CRT0      = /usr/lib/crt0-efi-x86_64.o

all: MyKernel.efi

MyKernel.efi: main.o
	ld $(LDFLAGS) $(CRT0) main.o -o MyKernel.efi
	objcopy -j .text -j .sdata -j .data -j .dynamic \
            -j .dynsym -j .rel* -j .rela* -j .reloc \
            --target=efi-app-$(ARCH) MyKernel.efi MyKernel.efi

main.o: main.c
	gcc $(CFLAGS) -c main.c -o main.o

clean:
	rm -f *.o *.efi
