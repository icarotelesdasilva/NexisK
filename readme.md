#    NexisK

A hobby operating system kernel built from scratch for the i386 architecture.

The project focuses on low-level kernel development, hardware interaction, interrupt management, memory management and the mechanisms required to build a kernel without relying on an external bootloader.

 # Quick Start

Build the kernel

mkdir build


cd build

cmake ..

make -j$(nproc)


# Run with QEMU

make run

Generate a clean build


rm -rf build

mkdir build

cd build

cmake ..

make -j$(nproc)

# Features

* Custom x86 bootloader
* Automatic kernel sector calculation
* i386 protected mode
* Global Descriptor Table (GDT)
* Interrupt Descriptor Table (IDT)
* Interrupt handlers
* Programmable Interrupt Controller (PIC)
* Programmable Interval Timer (PIT)
* VGA text output
* C and NASM kernel components
* Freestanding kernel environment
* QEMU development and debugging

# Boot Process

The system currently follows this basic boot sequence:

BIOS
 ↓
Bootloader
 ↓
Load kernel from disk
 ↓
Load GDT
 ↓
Enter protected mode
 ↓
Initialize protected-mode segments
 ↓
Initialize stack
 ↓
Jump to kernel
 ↓
_start
 ↓
kmain
 ↓
Kernel initialization

The bootloader calculates the number of sectors required by kernel.bin automatically during the build process.

This prevents the bootloader from becoming desynchronized from the actual kernel size as the kernel grows.

# Kernel

The kernel is written primarily in C, with NASM assembly used where direct processor control is required.

Current kernel components include:

kernel/
├── drivers/
├── handlers/
├── idt/
├── interrupts/
├── timer/
├── kernel.asm
└── kmain.c

The kernel currently initializes its low-level interrupt infrastructure before continuing with the main kernel initialization.

# Architecture

Current target:

Architecture: i386
Mode:         Protected Mode
Language:     C / NASM
Boot:         Custom bootloader
Emulator:     QEMU
Build system: CMake

# Debugging

QEMU can be used to capture CPU exceptions and reset information:

qemu-system-i386 \
    -drive format=raw,file=os-image.bin,if=floppy \
    -d int,cpu_reset,guest_errors \
    -D logs_completos.txt \
    -no-reboot \
    -no-shutdown

The generated log is useful for diagnosing exceptions, triple faults and unexpected CPU resets.

# Memory

Memory management is part of the kernel’s planned low-level infrastructure.

Planned work includes:

* A20 handling
* BIOS memory map
* Physical Memory Manager (PMM)
* Paging
* Virtual Memory Manager (VMM)
* Kernel memory layout
* Proper BSS initialization

Roadmap

* [x]	Custom bootloader
* [x]	Kernel loading
* [x]	Protected mode
* [x]	GDT
* [x]	IDT
* [x]	Interrupt handlers
* [x]	PIC
* [x]	PIT
* [x]	VGA output
* [x]	Automatic kernel sector calculation
* [ ]	A20 handling
* [ ]	EDD/LBA disk loading
* [ ]	BIOS memory map
* [ ]	Physical memory manager
* [ ]	Paging
* [ ]	Virtual memory manager
* [ ]	Expand kernel subsystems

# Requirements

A Linux-based environment with:

* GCC
* NASM
* CMake
* QEMU

# Status

This is a hobby kernel under active development.

The architecture and internal interfaces are expected to change as new kernel subsystems are implemented.

# License

See the LICENSE file for the license used by this project.
