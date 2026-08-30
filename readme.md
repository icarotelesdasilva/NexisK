#    NexisK

A general-purpose kernel built from scratch for the i386 architecture.
The project focuses on low-level kernel development, hardware interaction, interrupt management, memory management and the mechanisms required to build a kernel without relying on an external bootloader.

## Quick Start

### Build the kernel:

```
mkdir build
cd build
cmake ..
make -j$(nproc)⁠
```

### Run with QEMU

```⁠make run```⁠

### Generate a clean build:

```⁠rm -rf build```⁠

## Features

- Custom x86 bootloader
- Automatic kernel sector calculation
- i386 protected mode
- Global Descriptor Table (GDT)
- Interrupt Descriptor Table (IDT)
- Interrupt handlers
- Programmable Interrupt Controller (PIC)
- Programmable Interval Timer (PIT)
- VGA text output
- C and NASM kernel components
- Freestanding kernel environment
- serial added
- interrupts on

## Boot Process

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

## Kernel
The kernel is written primarily in C, with NASM assembly used where direct processor control is required.

Current kernel components include:

```
kernel/

├── drivers/

├── handlers/

├── idt/

├── interrupts/

├── timer/

├── kernel.asm

└── kmain.c
```
The kernel currently initializes its low-level interrupt infrastructure before continuing with the main kernel initialization.

## Architecture
Current target:

Architecture: i386

Mode:         Protected Mode

Language:     C / NASM

Boot:         Custom bootloader

Emulator:     QEMU

Build system: CMake
⁠

## Memory

Memory management is currently under development.

## Roadmap

- [x]	Custom bootloader
- [x]	Kernel loading
- [x]	Protected mode
- [x]	GDT
- [x]	IDT
- [x]	Interrupt handlers
- [x]	PIC
- [x]	PIT
- [x]	VGA output
- [x]	Automatic kernel sector calculation
- [x] context switch
## Requirements

- Linux
- GCC
- NASM
- CMake
- QEMU

## License

See the `LICENSE` file for the license used by this project.
