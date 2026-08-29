ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

CFLAGS = -m32 \
         -ffreestanding \
         -fno-pie \
         -fno-stack-protector \
         -fno-asynchronous-unwind-tables \
         -fno-unwind-tables \
         -nostdlib \
         -c \
         -MMD

C_SOURCES = $(shell find kernel -type f -name '*.c')
ASM_SOURCES = $(shell find kernel -type f -name '*.asm')

OBJECTS = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.asm=.o)
DEPS = $(C_SOURCES:.c=.d)

all: os-image.bin

-include $(DEPS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.asm
	$(ASM) -f elf32 $< -o $@

boot.bin: boot/boot.asm
	$(ASM) -f bin $< -o $@

kernel.elf: $(OBJECTS) linker.ld
	$(LD) -m elf_i386 -T linker.ld -o $@ $(OBJECTS)

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

os-image.bin: boot.bin kernel.bin
	cat $^ > $@
	truncate -s 1474560 $@

run: os-image.bin
	qemu-system-i386 -drive format=raw,file=$<,if=floppy

clean:
	rm -f boot.bin kernel.elf kernel.bin os-image.bin
	find kernel -type f \( -name '*.o' -o -name '*.d' \) -delete