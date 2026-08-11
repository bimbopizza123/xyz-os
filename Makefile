CC = gcc
CFLAGS = -m32 -ffreestanding -c
ASM = nasm
ASMFLAGS = -f elf32
LD = ld
LDFLAGS = -m elf_i386 -T linker.ld

all: kernel.bin

boot.o: src/boot.asm
	$(ASM) $(ASMFLAGS) src/boot.asm -o boot.o

kernel.o: src/kernel.c
	$(CC) $(CFLAGS) src/kernel.c -o kernel.o

kernel.bin: boot.o kernel.o
	$(LD) $(LDFLAGS) -o kernel.bin boot.o kernel.o

run: kernel.bin
	qemu-system-i386 -kernel kernel.bin

clean:
	rm -f *.o *.bin *.img