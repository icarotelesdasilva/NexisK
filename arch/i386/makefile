TARGET = OS
ISO_FILE = $(TARGET).iso

SRC_DIR = .
OBJ_DIR = obj
ISO_DIR = isodir
BOOT_DIR = $(ISO_DIR)/boot
GRUB_DIR = $(BOOT_DIR)/grub

CC = gcc
AS = nasm
LD = ld

CFLAGS = -m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra
ASFLAGS = -f elf32

LDFLAGS = -m elf_i386 -nostdlib -T linker.ld

SRCS_C   := $(sort $(shell find . -type f -name '*.c' ! -path "*/$(OBJ_DIR)/*" ! -path "*/$(ISO_DIR)/*"))
SRCS_ASM := $(sort $(shell find . -type f -name '*.asm' ! -path "*/$(OBJ_DIR)/*" ! -path "*/$(ISO_DIR)/*"))

SRCS_C   := $(patsubst ./%, %, $(SRCS_C))
SRCS_ASM := $(patsubst ./%, %, $(SRCS_ASM))

OBJS_C   := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS_C))
OBJS_ASM := $(patsubst %.asm, $(OBJ_DIR)/%.o, $(SRCS_ASM))
OBJS     := $(sort $(OBJS_C) $(OBJS_ASM))

all: $(ISO_FILE)

$(ISO_FILE): $(BOOT_DIR)/$(TARGET).bin grub.cfg
	@mkdir -p $(GRUB_DIR)
	@cp grub.cfg $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

$(BOOT_DIR)/$(TARGET).bin: $(OBJS) linker.ld
	@mkdir -p $(BOOT_DIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(ISO_DIR) $(ISO_FILE) kernel.o

.PHONY: all clean

