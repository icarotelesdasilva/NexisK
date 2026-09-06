# NexisK

<p align="center">
  <strong>An experimental x86 kernel built from scratch.</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Architecture-i386-informational?style=flat-square" alt="Architecture">
  <img src="https://img.shields.io/badge/Language-C%20%2F%20NASM-blue?style=flat-square" alt="Language">
  <img src="https://img.shields.io/badge/Build-Make-000000?style=flat-square" alt="Build">
  <img src="https://img.shields.io/badge/Tested-QEMU%20%2B%20Real%20Hardware-orange?style=flat-square" alt="Testing">
  <img src="https://img.shields.io/badge/License-GPL--2.0-green?style=flat-square" alt="License">
  <img src="https://img.shields.io/badge/Status-Experimental-orange?style=flat-square" alt="Status">
</p>

<p align="center">
  <strong>Current Release: v0.8.8</strong>
</p>

---

## Overview

NexisK is an experimental operating system kernel developed from scratch for the i386 architecture.

The project focuses on low-level systems programming, x86 architecture, hardware interaction, interrupt handling, memory management, process infrastructure, kernel initialization and bootloader development.

NexisK is written primarily in C and NASM assembly and does not rely on an external bootloader such as GRUB or Limine.

The project uses a custom BIOS bootloader responsible for initializing the machine, detecting the physical memory map and loading the kernel.

NexisK is the kernel. The bootloader is maintained as a separate component.

The project is experimental and actively evolving.

---

## What Is NexisK?

NexisK is a kernel development project, not a complete operating-system distribution.

Its purpose is to explore the mechanisms involved in implementing a kernel, including:

* CPU initialization
* x86 protected mode
* Interrupt handling
* Hardware interrupts
* Physical memory discovery
* Physical memory management
* Virtual memory
* Privilege levels
* System calls
* Process management
* Context switching
* Hardware interaction
* Kernel and user-space boundaries

The current target architecture is 32-bit x86 / i386.

The project is intentionally developed from the lowest levels upward.

```text
                     NexisK Kernel
                          │
          ┌───────────────┼───────────────┐
          │               │               │
          ▼               ▼               ▼
         CPU          Interrupts       Drivers
          │               │               │
          └───────────────┼───────────────┘
                          │
                          ▼
                  Memory Discovery
                          │
                          ▼
                  Memory Management
                          │
                          ▼
                      Processes
                          │
                          ▼
                  Context Switching
                          │
                          ▼
                    System Calls
```

Some subsystems are still under active reconstruction and development.

---

## Project Scope

NexisK focuses specifically on kernel-level functionality.

It is not currently intended to provide:

* A complete desktop environment
* A complete Linux-like userland
* A general-purpose distribution
* A production-ready operating system
* A complete filesystem ecosystem

The primary goal is to develop a small, understandable and progressively more capable kernel.

---

## Current Status

The current development priority is building a reliable bare-metal kernel and boot architecture.

The kernel currently provides infrastructure for:

* Custom BIOS bootloader
* Two-stage boot structure
* Boot menu
* Kernel selection
* Kernel loading
* Bootable disk image generation
* Bootable ISO generation
* Real hardware boot validation
* Global Descriptor Table
* Interrupt Descriptor Table
* CPU exception handlers
* Hardware interrupt handling
* Programmable Interrupt Controller
* Programmable Interval Timer
* Keyboard interrupt handling
* PS/2 mouse interrupt handling
* VGA text output
* Serial output
* Basic system-call infrastructure
* Initial process/context infrastructure
* BIOS E820 memory map detection
* Kernel-side memory map reporting
* Initial Physical Memory Manager
* Initial PMM bitmap representation

The current memory-management implementation is being rebuilt incrementally.

The current PMM stage consumes the E820 memory map supplied by the bootloader and identifies usable physical memory regions.

The PMM currently:

* Reads the E820 entry count supplied by the bootloader
* Reads the E820 memory map
* Identifies entries with `Type 1`
* Calculates the number of 4 KiB pages in usable regions
* Iterates through usable memory in 4 KiB page increments
* Calculates a bitmap index for each physical page
* Marks the corresponding usable pages in the initial PMM bitmap representation
* Reports detected usable memory regions through serial output

Full page allocation, freeing and complete reservation handling are still under development.

---

# Features

## Custom BIOS Bootloader

NexisK uses its own x86 BIOS bootloader instead of relying on an external bootloader.

The boot architecture is divided into two stages:

```text
BIOS
 │
 ▼
Stage 1
 │
 ▼
Stage 2
 │
 ├── Boot initialization
 ├── Boot menu
 ├── Kernel selection
 ├── E820 memory detection
 └── Kernel loading
       │
       ▼
   NexisK Kernel
```

The bootloader currently supports:

* BIOS boot
* Two-stage boot process
* Boot menu
* Kernel selection
* Kernel loading
* E820 memory map detection
* Bootable disk image generation
* Bootable ISO generation
* Real hardware boot

The bootloader is maintained separately from the kernel implementation.

---

## CPU and Protected Mode

NexisK targets the i386 architecture and executes the kernel in 32-bit protected mode.

Current CPU-related infrastructure includes:

* i386 target
* Protected-mode execution
* Global Descriptor Table
* Segment configuration
* Kernel execution environment

Future work includes expanding privilege-level support and improving CPU initialization.

---

## Interrupts

The kernel contains an Interrupt Descriptor Table and infrastructure for processor exceptions and hardware interrupts.

Current interrupt-related components include:

* IDT
* CPU exception handlers
* PIC remapping
* PIT
* Keyboard interrupts
* PS/2 mouse interrupts
* System-call interrupt vector

The interrupt subsystem provides the foundation required for future scheduling, process management and additional hardware drivers.

---

## System Calls

NexisK contains a basic system-call mechanism using:

```text
int 0x80
```

The syscall number is passed through the `EAX` register.

The current implementation is intentionally minimal and is primarily used to validate the kernel system-call path.

```text
Execution Context
       │
       │ int 0x80
       ▼
    IDT[0x80]
       │
       ▼
 syscall handler
       │
       ▼
 Kernel syscall
       │
       ▼
      iret
```

The syscall interface will evolve alongside process and privilege-level support.

---

## Kernel I/O

NexisK currently provides basic low-level I/O facilities including:

* VGA text output
* Serial output
* Keyboard input
* PS/2 mouse input

Serial output is particularly useful for debugging kernel behavior under QEMU.

---

# Memory Management

## E820 Memory Discovery

NexisK uses the BIOS `INT 15h, E820h` interface to discover the physical memory map during boot.

The bootloader collects the memory regions and exposes the resulting map to the kernel.

Example output:

```text
kernel alive.

Base: 0x0000000000000000 | Size: 0x000000000009FC00 | Type: 0x00000001
Base: 0x000000000009FC00 | Size: 0x0000000000000400 | Type: 0x00000002
Base: 0x00000000000F0000 | Size: 0x0000000000010000 | Type: 0x00000002
Base: 0x0000000000100000 | Size: 0x0000000007EE0000 | Type: 0x00000001
Base: 0x0000000007FE0000 | Size: 0x0000000000020000 | Type: 0x00000002
Base: 0x00000000FFFC0000 | Size: 0x0000000000040000 | Type: 0x00000002

Free Memory Region Found:
Base Address: 0x0000000000000000
Size: 0x000000000009FC00

Free Memory Region Found:
Base Address: 0x0000000000100000
Size: 0x0000000007EE0000
```

The kernel identifies usable E820 regions and processes them in 4 KiB page increments.

---

## Physical Memory Manager

NexisK contains the initial implementation of its Physical Memory Manager.

The PMM receives the E820 memory map and identifies usable physical memory.

Its current basic flow is:

```text
Bootloader
    │
    ▼
 BIOS E820
    │
    ▼
E820 Memory Map
    │
    ▼
NexisK Kernel
    │
    ▼
 pmm_init()
    │
    ├── Read E820 entries
    ├── Find Type 1 regions
    ├── Calculate 4 KiB pages
    ├── Calculate physical page addresses
    ├── Calculate bitmap indices
    └── Mark usable pages in bitmap
```

The current PMM implementation uses an initial byte-per-page bitmap representation.

Each bitmap entry corresponds to one 4 KiB physical page.

The current representation uses:

```text
0 = free
1 = reserved / occupied
```

Usable E820 `Type 1` regions are processed and their corresponding page entries are marked as free.

The current PMM stage has been validated by successfully processing the E820 map and identifying the usable physical memory regions reported by the bootloader.

The following stages are still under development:

* Complete bitmap initialization
* Physical page state tracking
* Kernel memory reservation
* Bootloader memory reservation
* Bitmap memory reservation
* Page blocking/reservation
* Physical page allocation
* Physical page freeing
* Better handling of memory-map boundaries
* Integration with the Virtual Memory Manager

---

## Virtual Memory

Virtual memory is planned to be built on top of the physical memory manager.

Planned functionality includes:

* Paging
* Virtual address mapping
* Page fault handling
* Dynamic page mapping
* Kernel/user memory permissions
* Per-process address spaces

The VMM is not yet considered complete.

---

# Process Infrastructure

The repository contains initial process-related infrastructure and context-switching groundwork.

This subsystem is still under development and will evolve alongside memory management and privilege-level support.

Planned functionality includes:

* Process creation
* Process destruction
* PID management
* Address spaces
* Context switching
* Scheduler
* Preemptive multitasking
* Process isolation

---

# Architecture

| Component               | Current Implementation               |
| ----------------------- | ------------------------------------ |
| Project                 | NexisK Kernel                        |
| Architecture            | i386 / x86-32                        |
| CPU Mode                | Protected Mode                       |
| Kernel Language         | C                                    |
| Assembly                | NASM                                 |
| Bootloader              | Custom BIOS bootloader               |
| Boot Structure          | Stage 1 + Stage 2                    |
| Boot Menu               | Yes                                  |
| Memory Discovery        | BIOS E820                            |
| Physical Memory Manager | Initial implementation               |
| PMM Bitmap              | Initial byte-per-page representation |
| Virtual Memory Manager  | In development                       |
| Paging                  | In development                       |
| Interrupts              | IDT + PIC                            |
| Timer                   | PIT                                  |
| Display                 | VGA text mode                        |
| Debug Output            | Serial                               |
| Input                   | Keyboard / PS/2 mouse                |
| Syscalls                | `int 0x80`                           |
| Process Infrastructure  | Initial                              |
| Emulator                | QEMU                                 |
| Physical Testing        | Real x86 hardware                    |
| Build System            | GNU Make                             |
| License                 | GPL-2.0-only                         |

---

# Kernel Architecture

The kernel is organized into independent subsystems.

```text
CPU
 │
 ├── GDT
 ├── Protected Mode
 └── Privilege Infrastructure
       │
       ▼
Interrupts
 │
 ├── Exceptions
 ├── IRQs
 ├── PIC
 └── PIT
       │
       ▼
Memory
 │
 ├── E820
 ├── PMM
 ├── VMM
 └── Paging
       │
       ▼
Processes
 │
 ├── Address Spaces
 ├── Context Switching
 └── Scheduler
       │
       ▼
System Calls
       │
       ▼
User Space
```

The architecture is expected to change as the project evolves.

---

# Project Structure

```text
NexisK/
│
├── boot/
│   ├── stage1.S
│   └── stage2.S
│
├── kernel/
│   ├── drivers/
│   │   ├── kernel_panic.c
│   │   ├── mouse.c
│   │   ├── serial.asm
│   │   ├── serial_print.c
│   │   └── vga.c
│
│   ├── gdt/
│   │   ├── gdt.asm
│   │   └── gdt.c
│
│   ├── handlers/
│   │   ├── handler_0x00.c
│   │   ├── handler_0x08.c
│   │   ├── handler_central.asm
│   │   ├── handler_irq0.c
│   │   ├── interrupt.h
│   │   ├── keyboard_handler.c
│   │   ├── syscall.asm
│   │   └── syscall.c
│
│   ├── idt/
│   │   ├── idt.asm
│   │   ├── idt.c
│   │   └── idt.h
│
│   ├── interrupts/
│   │   ├── io.h
│   │   ├── pic.c
│   │   └── pic.h
│
│   ├── memory/
│   │   ├── memory_learn.h
│   │   ├── pmm.h
│   │   └── memory_manager/
│   │       ├── memory.c
│   │       └── pmm.c
│
│   ├── process/
│   │   ├── process.c
│   │   └── process.h
│
│   ├── timer/
│   │   ├── pit.c
│   │   └── pit.h
│
│   └── kmain.c
│
├── LICENSE
├── linker.ld
├── makefile
└── readme.md
```

---

# Build System

NexisK uses a custom GNU Make build system.

The Makefile automatically:

1. Compiles C kernel sources.
2. Assembles NASM sources.
3. Builds the boot stages.
4. Links the kernel and boot components.
5. Generates the bootable disk image.
6. Creates the bootable ISO.

The kernel is compiled as a freestanding 32-bit i386 target.

Important compiler options include:

```text
-m32
-march=i386
-ffreestanding
-fno-pie
-fno-stack-protector
-fno-builtin
-nostdlib
-nodefaultlibs
```

This keeps the kernel independent from the host operating system's standard runtime.

---

# Requirements

NexisK is currently developed and tested primarily on Linux.

Required tools:

* GCC
* NASM
* GNU Make
* GNU ld
* QEMU
* genisoimage

On Debian or Ubuntu:

```bash
sudo apt update

sudo apt install \
    build-essential \
    gcc \
    nasm \
    make \
    binutils \
    qemu-system-x86 \
    genisoimage
```

---

# Building

Clone the repository:

```bash
git clone https://github.com/icarotelesdasilva/NexisK.git
cd NexisK
```

Build the kernel and bootable ISO:

```bash
make
```

Generated files are placed inside:

```text
build/
```

The main bootable image is:

```text
build/NexisK.iso
```

---

# Running with QEMU

Run NexisK using:

```bash
make run
```

This starts:

```text
qemu-system-i386
```

with the generated ISO attached as a CD-ROM.

Serial output is connected to the terminal and is used extensively for kernel debugging.

---

# Development Debug Mode

For deeper QEMU diagnostics:

```bash
make dev
```

The debug configuration writes the QEMU log to:

```text
build/qemu.log
```

This is useful for investigating:

* CPU resets
* Interrupts
* Guest errors
* Unimplemented instructions
* MMU activity
* Protected-mode execution
* Kernel execution

---

# Cleaning the Build

To remove generated build artifacts:

```bash
make clean
```

A clean build can then be produced with:

```bash
make
```

---

# Boot Process

The current boot process is approximately:

```text
                         BIOS
                           │
                           ▼
                        Stage 1
                           │
                           ▼
                        Stage 2
                           │
              ┌────────────┼────────────┐
              │            │            │
              ▼            ▼            ▼
        Initialization  Boot Menu   E820 Detection
                                      │
                                      ▼
                                E820 Memory Map
                                      │
                                      ▼
                                Kernel Loading
                                      │
                                      ▼
                                  NexisK Kernel
                                      │
                                      ▼
                                    kmain
                                      │
                         ┌────────────┼────────────┐
                         │            │            │
                         ▼            ▼            ▼
                        GDT          IDT          PMM
                         │            │            │
                         └────────────┼────────────┘
                                      │
                                      ▼
                                Kernel Runtime
```

The bootloader is responsible for initializing the machine, detecting the available physical memory map, loading the kernel and transferring control to it.

The kernel then initializes its own runtime subsystems.

---

# Bootable Image

The build system creates a raw boot image and packages it into an ISO.

Generated files include:

```text
build/
├── stage1.bin
├── stage2.bin
├── disk.img
├── iso/
│   └── boot.img
└── NexisK.iso
```

The boot image is currently created using a 1.44 MB disk-image layout and packaged as an El Torito bootable ISO.

---

# Development Roadmap

## Bootloader

* [x] Custom BIOS bootloader
* [x] Stage 1
* [x] Stage 2
* [x] Boot menu
* [x] Kernel selection
* [x] Kernel loading
* [x] E820 memory map detection
* [x] Bootable disk image
* [x] Bootable ISO
* [x] Real hardware validation
* [ ] More robust disk access
* [ ] LBA-based loading
* [ ] Improved hardware compatibility
* [ ] UEFI support

## CPU and Privilege Management

* [x] i386 target
* [x] Protected-mode foundation
* [x] GDT
* [ ] TSS improvements
* [ ] Ring 3
* [ ] Kernel/user privilege separation
* [ ] Improved CPU initialization

## Interrupts

* [x] IDT
* [x] CPU exception handlers
* [x] PIC
* [x] PIT
* [x] Keyboard interrupts
* [x] PS/2 mouse interrupts
* [x] System-call interrupt
* [ ] More complete interrupt abstraction
* [ ] Improved interrupt dispatching

## Memory Management

* [x] BIOS E820 memory map detection
* [x] Kernel-side E820 processing
* [x] Initial Physical Memory Manager
* [x] Detection of usable physical memory regions
* [x] 4 KiB page iteration
* [x] Initial PMM bitmap representation
* [x] Usable-page bitmap marking
* [ ] Complete bitmap initialization
* [ ] Physical page state tracking
* [ ] Page blocking/reservation
* [ ] Physical page allocation
* [ ] Physical page freeing
* [ ] Kernel memory reservation
* [ ] Bootloader memory reservation
* [ ] Bitmap memory reservation
* [ ] Virtual Memory Manager
* [ ] Paging
* [ ] Dynamic page mapping
* [ ] Page fault handling
* [ ] Kernel/user memory permissions
* [ ] Per-process address spaces

## Processes

* [x] Initial process infrastructure
* [x] Initial context-switching infrastructure
* [ ] Process creation
* [ ] Process destruction
* [ ] PID management
* [ ] Process address spaces
* [ ] Context switching
* [ ] Scheduler
* [ ] Preemptive multitasking
* [ ] Process isolation

## System Calls

* [x] Basic syscall interface
* [x] `int 0x80` entry
* [ ] Expanded syscall ABI
* [ ] Process-related syscalls
* [ ] Memory-related syscalls
* [ ] File-related syscalls
* [ ] User-space API

## Hardware

* [x] VGA text output
* [x] Serial output
* [x] Keyboard
* [x] PS/2 mouse
* [ ] Additional device drivers
* [ ] Storage drivers
* [ ] LBA support
* [ ] Hardware abstraction improvements

## Storage

* [ ] Disk abstraction
* [ ] Disk driver
* [ ] LBA support
* [ ] Filesystem abstraction
* [ ] Initial filesystem
* [ ] File operations

## Future Architecture

* [ ] More complete user-space support
* [ ] Stable kernel ABI
* [ ] Improved hardware abstraction
* [ ] UEFI boot support
* [ ] x86-64 transition

---

# Versioning

NexisK uses version numbers to track major development milestones.

## Current Release

**v0.8.8 — Initial PMM Bitmap Integration**

## Recent Milestones

| Version | Milestone                                               |
| ------- | ------------------------------------------------------- |
| v0.4.0  | Physical Memory Manager                                 |
| v0.5.0  | Virtual Memory / Paging / PS/2                          |
| v0.6.0  | GDT / Ring 3 / TSS                                      |
| v0.7.0  | Basic System Call Interface                             |
| v0.7.1  | Ring 3 Syscall Validation                               |
| v0.7.3  | Basic VMM Page Mapping and Boot/Build Refactoring       |
| v0.8.5  | Bootloader refactor and E820 memory map detection       |
| v0.8.6  | Verified E820 memory map reporting in the kernel        |
| v0.8.8  | Initial PMM bitmap integration and usable-page tracking |

Historical versions may contain kernel subsystems that are not present in the current implementation.

---

# Development Philosophy

NexisK is developed from the bottom up.

The project intentionally avoids hiding the underlying architecture behind large abstractions.

The general development path is:

```text
Bootloader
    │
    ▼
CPU Initialization
    │
    ▼
Protected Mode
    │
    ▼
Memory Discovery
    │
    ▼
Physical Memory Management
    │
    ▼
Virtual Memory
    │
    ▼
Interrupts
    │
    ▼
Privilege Levels
    │
    ▼
System Calls
    │
    ▼
Processes
    │
    ▼
Scheduling
    │
    ▼
User Space
```

Each layer is intended to provide the foundation required by the next.

This makes NexisK primarily a kernel development, learning and experimentation project.

---

# Design Goals

The long-term goals of NexisK include:

* Build a maintainable x86 kernel
* Understand the hardware/software boundary
* Implement core kernel subsystems from scratch
* Implement physical memory management
* Implement virtual memory
* Provide a clean process model
* Implement multitasking
* Establish a stable system-call interface
* Support user-space execution
* Improve hardware support
* Develop a robust boot architecture
* Eventually explore x86-64

The project prioritizes understanding, experimentation and incremental development over premature complexity.

---

# Limitations

NexisK is experimental and currently has several limitations.

## Architecture

The current kernel targets 32-bit i386/x86.

## Firmware

The current boot path is based on the traditional BIOS environment.

## Memory Management

The PMM is currently in its initial reimplementation stage.

It can consume the E820 memory map, identify usable physical memory regions, iterate through those regions in 4 KiB pages and maintain an initial bitmap representation.

Complete bitmap initialization, reservation of kernel and bootloader memory, physical page allocation and freeing are still under development.

Virtual memory and paging are also not yet complete.

## Privilege Levels

Full Ring 3 and kernel/user privilege separation remain under development.

## Process Management

Process infrastructure exists in an early stage and is not yet a complete multitasking system.

## Scheduling

A complete scheduler has not yet been implemented.

## Storage

Filesystem and general storage support are not currently available as complete kernel subsystems.

## Hardware Compatibility

Real hardware testing is performed, but compatibility with all x86 hardware is not guaranteed.

## Production Use

NexisK is not production-ready and should be considered an experimental kernel.

---

# Contributing

Contributions, experiments and technical discussions are welcome.

When contributing to NexisK:

1. Keep changes focused.
2. Preserve clear subsystem boundaries.
3. Avoid unnecessary dependencies.
4. Keep architecture-specific code explicit.
5. Test changes with QEMU.
6. Test hardware-sensitive changes on real hardware when possible.
7. Document significant architectural changes.
8. Prefer small and understandable commits.

For larger architectural changes, opening an issue before implementation is recommended.

---

# Debugging

Low-level kernel development often requires debugging below the level of traditional application development.

Useful areas to inspect include:

* BIOS boot behavior
* Stage 1 execution
* Stage 2 execution
* Kernel loading
* E820 memory map detection
* PMM initialization
* Physical memory regions
* PMM bitmap state
* Protected-mode initialization
* GDT initialization
* IDT initialization
* Interrupt delivery
* PIC configuration
* PIT configuration
* Keyboard interrupts
* PS/2 mouse interrupts
* System-call entry
* CPU resets
* Hardware-specific behavior

When using QEMU, the `make dev` target provides additional diagnostic information through the QEMU log.

Serial output can also be used to trace kernel execution without relying exclusively on VGA output.

---

# License

NexisK is licensed under the GNU General Public License v2.0 only (`GPL-2.0-only`).

See [LICENSE](LICENSE) for the complete license text.

---

# Author

Developed by `icarotelesdasilva`.

GitHub repository:

https://github.com/icarotelesdasilva/NexisK

---

# Final Note

NexisK is a kernel built to explore what happens underneath an operating system.

It is intentionally developed close to the hardware, from the boot process and CPU initialization to interrupts, memory discovery, physical memory management, processes and system calls.

The project is still evolving, and its architecture will continue to change as new kernel subsystems are implemented.

<p align="center">
  <strong>NexisK — An experimental kernel built from the ground up.</strong>
</p>