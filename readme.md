# NexisK

A general-purpose kernel built from scratch for the **i386 architecture**.

NexisK is an experimental operating system kernel focused on low-level development, x86 protected mode, hardware interaction, interrupt handling, memory management, privilege levels, system calls and process-related mechanisms.

The project is written from scratch using **C and NASM assembly**, with a custom bootloader and no external bootloader dependency.

## Current Status

NexisK currently boots into **i386 protected mode** and provides a functional low-level kernel foundation including:

* Custom bootloader
* Automatic kernel sector calculation
* Protected mode
* Global Descriptor Table (GDT)
* Task State Segment (TSS)
* Ring 3 support
* Interrupt Descriptor Table (IDT)
* Interrupt handlers
* Programmable Interrupt Controller (PIC)
* Programmable Interval Timer (PIT)
* PS/2 mouse interrupt handling
* VGA text output
* Serial output
* Physical Memory Manager (PMM)
* Virtual Memory Manager (VMM)
* Paging
* Basic context switching
* Basic system call interface
* Ring 3 → `int 0x80` → kernel syscall execution

The project is still under active development and should be considered experimental.

## Quick Start

### Requirements

* Linux
* GCC
* NASM
* CMake
* QEMU

### Build

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Run

```bash
make run
```

The kernel is executed using QEMU with serial output available for debugging.

### Clean Build

```bash
rm -rf build
mkdir build
cd build
cmake ..
make -j$(nproc)
```

## Architecture

| Component            | Current implementation |
| -------------------- | ---------------------- |
| Architecture         | i386                   |
| CPU mode             | Protected Mode         |
| Language             | C / NASM               |
| Boot                 | Custom bootloader      |
| Memory management    | PMM + initial VMM      |
| Paging               | Enabled                |
| Privilege levels     | Ring 0 / Ring 3        |
| Interrupt controller | PIC                    |
| Timer                | PIT                    |
| Emulator             | QEMU                   |
| Build system         | CMake                  |
| License              | GPL-2.0-only           |

## Features

### Bootloader

NexisK uses a custom x86 bootloader instead of relying on GRUB or another external bootloader.

The build system calculates the number of sectors required by the kernel automatically. This keeps the bootloader synchronized with the generated kernel image as the kernel grows.

### Protected Mode

The kernel enters i386 protected mode during the boot process and establishes the required protected-mode segments before transferring execution to the kernel.

### GDT and TSS

The kernel contains its own Global Descriptor Table implementation.

Current GDT support includes:

* Kernel code segment
* Kernel data segment
* User code segment
* User data segment
* Task State Segment descriptor
* Ring 3 privilege support

The TSS provides the kernel stack information required when transitioning from user mode back into Ring 0.

### Interrupts

The kernel contains an Interrupt Descriptor Table and interrupt handlers for processor exceptions and hardware interrupts.

Current infrastructure includes:

* IDT
* CPU exception handlers
* PIC remapping
* PIT
* Keyboard interrupt handling
* PS/2 mouse interrupt handling
* System call interrupt vector

### System Calls

NexisK currently implements a basic system call mechanism through:

```asm
int 0x80
```

The syscall number is passed through `EAX`.

The current implementation contains a basic syscall used to validate the complete Ring 3 → Ring 0 transition and return path.

Current execution path:

```text
Ring 3
  |
  | int 0x80
  v
IDT[0x80]
  |
  v
syscall_handler
  |
  v
kernel syscall
  |
  v
iret
  |
  v
Ring 3
```

The syscall interface is intentionally minimal at this stage.

### Memory Management

NexisK currently contains two layers of memory management.

#### Physical Memory Manager

The PMM uses the memory map provided during boot to identify available physical memory.

Current functionality includes:

* Physical page discovery
* Bitmap-based page tracking
* Page allocation
* Page freeing
* Bitmap protection

#### Virtual Memory Manager

The VMM currently provides the initial paging infrastructure.

Current implementation:

* Page directory creation
* Page table creation
* Identity mapping
* CR3 initialization
* Paging activation through CR0

The current VMM is intentionally simple and is planned to evolve into a full virtual memory subsystem.

## Context Switching

NexisK contains initial process/context-switching infrastructure.

The context-switch mechanism preserves processor execution state so execution can later continue from the saved context.

This subsystem is still being developed toward a complete process and scheduling architecture.

## Kernel Structure

```text
kernel/
├── GDT/
│   ├── gdt.c
│   ├── gdt.h
│   ├── gdt.asm
│   └── gdt_flush.s
│
├── drivers/
│   ├── mouse.c
│   ├── serial.asm
│   ├── serial_print.c
│   └── vga.c
│
├── handlers/
│   ├── handler_0x00.c
│   ├── handler_0x08.c
│   ├── handler_central.asm
│   ├── handler_irq0.c
│   ├── keyboard_handler.c
│   ├── syscall.asm
│   └── syscall.c
│
├── idt/
│   ├── idt.c
│   ├── idt.h
│   └── idt.asm
│
├── interrupts/
│   ├── pic.c
│   └── ...
│
├── memory/
│   ├── pmm.c
│   └── pmm.h
│
├── process/
│   ├── process.c
│   └── process.h
│
├── timer/
│   └── pit.c
│
├── vmm/
│   ├── vmm.c
│   └── vmm.h
│
├── kernel.asm
└── kmain.c
```

The repository currently reflects this modular structure, including dedicated GDT, memory, VMM and process subsystems.

## Boot Process

The current boot flow is approximately:

```text
BIOS
 |
 v
Custom Bootloader
 |
 v
Load Kernel
 |
 v
Load / Initialize GDT
 |
 v
Enter Protected Mode
 |
 v
Initialize Kernel Stack
 |
 v
_start
 |
 v
kmain
 |
 +--> GDT / TSS
 |
 +--> PIC
 |
 +--> VMM / Paging
 |
 +--> IDT
 |
 +--> PIT
 |
 +--> PS/2
 |
 +--> PMM
 |
 +--> Kernel initialization
```

## Development Roadmap

### Completed

* [x] Custom bootloader
* [x] Automatic kernel sector calculation
* [x] Kernel loading
* [x] i386 protected mode
* [x] GDT
* [x] IDT
* [x] CPU exception handlers
* [x] PIC
* [x] PIT
* [x] VGA output
* [x] Serial output
* [x] Keyboard interrupt handling
* [x] PS/2 mouse interrupt handling
* [x] Physical Memory Manager
* [x] Physical page allocation
* [x] Physical page freeing
* [x] Initial Virtual Memory Manager
* [x] Paging
* [x] TSS
* [x] Ring 3
* [x] Basic syscall interface
* [x] Ring 3 syscall execution
* [x] Initial context-switching infrastructure

### Planned

* [ ] Expand virtual memory management
* [ ] User/kernel page permission separation
* [ ] Dynamic virtual page mapping
* [ ] Process address spaces
* [ ] Process creation
* [ ] Scheduler
* [ ] Multitasking
* [ ] More system calls
* [ ] User-space programs
* [ ] Filesystem
* [ ] Storage drivers
* [ ] Transition toward x86-64

## Versioning

NexisK uses semantic versioning for major development milestones.

Recent milestones include:

```text
v0.4.0  Physical Memory Manager
v0.5.0  Virtual Memory / Paging / PS/2
v0.6.0  GDT / Ring 3 / TSS
v0.7.0  Basic Syscall Interface
v0.7.1  Ring 3 Syscall Validation
```

## Project Goals

The long-term goal of NexisK is to evolve from a minimal i386 kernel into a more complete operating system kernel while keeping the implementation understandable and developed from the lowest levels upward.

The project is primarily a learning and experimentation project focused on understanding:

* x86 architecture
* CPU privilege levels
* Interrupts
* Memory management
* Paging
* Processes
* Context switching
* System calls
* Kernel/user-space boundaries
* Operating system internals

## License

NexisK is licensed under the **GNU General Public License v2.0 only**.

See [`LICENSE`](LICENSE) for the complete license text.
