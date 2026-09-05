# NexisK

<p align="center">
  <strong>An experimental x86 kernel built from scratch.</strong>
</p>

<p align="center">
  <a href="#overview">Overview</a> •
  <a href="#features">Features</a> •
  <a href="#architecture">Architecture</a> •
  <a href="#testing">Testing</a> •
  <a href="#building">Building</a> •
  <a href="#boot-process">Boot Process</a> •
  <a href="#roadmap">Roadmap</a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Architecture-i386-informational?style=flat-square" alt="Architecture">
  <img src="https://img.shields.io/badge/Language-C%20%2F%20NASM-blue?style=flat-square" alt="Language">
  <img src="https://img.shields.io/badge/Build-Make-000000?style=flat-square" alt="Make">
  <img src="https://img.shields.io/badge/Tested-QEMU%20%2B%20Real%20Hardware-orange?style=flat-square" alt="Testing">
  <img src="https://img.shields.io/badge/License-GPL--2.0-green?style=flat-square" alt="License">
  <img src="https://img.shields.io/badge/Status-Experimental-orange?style=flat-square" alt="Status">
</p>

<p align="center">
  <strong>Current Release: <a href="https://github.com/icarotelesdasilva/NexisK/releases/tag/v0.8.6">v0.8.6</a></strong>
</p>

## Overview

NexisK is an experimental operating system kernel developed from scratch for the i386 architecture.

The project focuses on low-level systems programming, x86 architecture, hardware interaction, interrupt handling, kernel initialization, bootloader development and the foundations required to build a kernel from the ground up.

NexisK is written primarily in C and NASM assembly and is developed without relying on an external bootloader such as GRUB or Limine.

The repository also contains a custom BIOS bootloader responsible for initializing the machine, presenting a boot menu, detecting the system memory map and loading the selected kernel.

NexisK is the kernel. The bootloader is a separate component used to bootstrap it.

The project is experimental and actively evolving.

## What Is NexisK?

NexisK is a kernel development project, not a complete operating-system distribution.

Its purpose is to explore the mechanisms involved in implementing a kernel, including:

* CPU initialization
* x86 protected mode
* Interrupt handling
* Hardware interrupts
* Memory discovery
* Memory management
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
       ┌─────────────────┼─────────────────┐
       │                 │                 │
       ▼                 ▼                 ▼
      CPU            Interrupts         Drivers
       │                 │                 │
       │                 │                 │
       └─────────────────┼─────────────────┘
                         │
                         ▼
                 Memory Discovery
                         │
                         ▼
                    Memory Mgmt
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

Some kernel subsystems are currently under reconstruction following the bootloader architecture refactor.

## Project Scope

NexisK focuses specifically on kernel-level functionality.

It is not currently intended to provide:

* A complete desktop environment
* A complete Linux-like userland
* A general-purpose distribution
* A production-ready operating system
* A complete filesystem ecosystem

The primary goal is to develop a small, understandable and progressively more capable kernel.

```text
NexisK
  │
  └── Kernel
       ├── CPU management
       ├── Interrupt handling
       ├── Memory discovery
       ├── Memory management
       ├── Process management
       ├── Scheduling
       ├── System calls
       └── Hardware interfaces
```

## Current Status

The current development priority is a reliable bare-metal boot path and bootloader architecture.

The kernel currently provides infrastructure for:

* Custom BIOS bootloader
* Two-stage boot structure
* Boot menu
* Kernel selection
* Kernel loading
* Bootable disk image generation
* Bootable ISO generation
* Real hardware boot validation
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
* Kernel-side reporting of detected memory regions

Several previously implemented kernel subsystems are currently temporarily removed while the new bootloader architecture is stabilized.

These include:

* GDT
* TSS
* Ring 3
* Physical Memory Manager
* Virtual Memory Manager
* Paging
* Dynamic virtual page mapping

The current E820 implementation is used for physical memory discovery at boot.

The detected memory map is now successfully passed into the kernel and reported through kernel output.

Full physical and virtual memory management are planned to be reintroduced separately.

## Features

### Custom BIOS Bootloader

NexisK uses its own x86 BIOS bootloader instead of relying on an external bootloader.

The current boot architecture is divided into two stages:

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
 ├── E820 memory map detection
 └── Kernel loading
       │
       ▼
   NexisK Kernel
```

The bootloader is designed to provide a minimal and controlled environment for starting the kernel.

It currently supports:

* BIOS boot
* Two-stage boot process
* Boot menu
* Kernel selection
* Kernel loading
* E820 memory map detection
* Bootable disk image generation
* Real hardware boot

The bootloader is maintained separately from the kernel implementation.

### Interrupts

The kernel contains an Interrupt Descriptor Table (IDT) and infrastructure for processor exceptions and hardware interrupts.

Current interrupt-related components include:

* IDT
* CPU exception handlers
* PIC remapping
* PIT
* Keyboard interrupt handling
* PS/2 mouse interrupt handling
* System-call interrupt vector

The interrupt subsystem provides the foundation required for future scheduling, process management and hardware drivers.

### System Calls

NexisK contains a basic system-call mechanism using:

```text
int 0x80
```

The syscall number is passed through the `EAX` register.

The current implementation is intentionally minimal and is primarily used to validate the kernel's system-call path.

Current execution flow:

```text
User / Test Context
       │
       │ int 0x80
       ▼
   IDT[0x80]
       │
       ▼
syscall_handler
       │
       ▼
 Kernel syscall
       │
       ▼
      iret
```

The system-call interface will be expanded as process and privilege-level infrastructure is rebuilt.

### Kernel I/O

NexisK currently provides basic low-level I/O facilities including:

* VGA text output
* Serial output
* Keyboard input
* PS/2 mouse input

Serial output is particularly useful for debugging kernel behavior under QEMU.

### Process Infrastructure

The repository contains initial process-related infrastructure and context-switching groundwork.

This subsystem is still under development and will evolve alongside memory management and privilege-level support.

### Memory Map Detection

NexisK uses the BIOS `INT 15h, E820h` interface to detect the system's physical memory map during boot.

The loader collects the returned memory regions and the kernel reports the detected entries.

Example output:

```text
kernel alive.

Base: 0x0000000000000000 | Size: 0x000000000009FC00 | Type: 0x00000001
Base: 0x000000000009FC00 | Size: 0x0000000000000400 | Type: 0x00000002
Base: 0x00000000000F0000 | Size: 0x0000000000010000 | Type: 0x00000002
Base: 0x0000000000100000 | Size: 0x0000000007EE0000 | Type: 0x00000001
Base: 0x0000000007FE0000 | Size: 0x0000000000020000 | Type: 0x00000002
Base: 0x00000000FFFC0000 | Size: 0x0000000000040000 | Type: 0x00000002
```

This provides the foundation for future physical memory management.

Planned memory-management work includes:

* Physical page allocation
* Physical page freeing
* Paging
* Virtual address mapping
* Page fault handling
* Kernel/user memory permissions
* Per-process address spaces

Full PMM and VMM implementations remain planned work.

## Testing

NexisK is tested in both virtualized environments and on real x86 hardware.

### QEMU

QEMU is the primary environment for rapid development, debugging and regression testing.

The project uses:

```text
qemu-system-i386
```

The normal development command is:

```bash
make run
```

A development/debugging mode is also available:

```bash
make dev
```

The debug target enables additional QEMU diagnostics and writes a log to:

```text
build/qemu.log
```

This makes it possible to investigate low-level problems involving:

* CPU resets
* Interrupts
* Guest errors
* Unimplemented instructions
* MMU activity
* Kernel execution

### Real Hardware

NexisK is also tested on real x86 PCs.

Real-hardware testing is an important part of the project because a kernel that works in an emulator is not automatically guaranteed to behave correctly on physical machines.

Testing on physical hardware helps validate behavior involving:

* BIOS implementations
* CPU initialization
* Hardware interrupts
* PIC/PIT behavior
* PS/2 devices
* VGA hardware
* Boot media
* Timing
* Hardware-specific behavior

The bootloader has been validated on real hardware.

Hardware compatibility is still evolving. Successful execution on one physical machine does not imply compatibility with every x86 PC.

## Architecture

| Component               | Current Implementation |
| ----------------------- | ---------------------- |
| Project                 | NexisK Kernel          |
| Architecture            | i386 / x86-32          |
| CPU Mode                | Protected Mode         |
| Kernel Language         | C                      |
| Assembly                | NASM                   |
| Bootloader              | Custom BIOS bootloader |
| Boot Structure          | Stage 1 + Stage 2      |
| Boot Menu               | Yes                    |
| Memory Map              | BIOS E820              |
| Interrupts              | IDT + PIC              |
| Timer                   | PIT                    |
| Display                 | VGA text mode          |
| Debug Output            | Serial                 |
| Input                   | Keyboard / PS/2 mouse  |
| Syscalls                | `int 0x80`             |
| Process Infrastructure  | Initial                |
| Physical Memory Manager | Temporarily removed    |
| Virtual Memory Manager  | Temporarily removed    |
| Paging                  | Temporarily removed    |
| Ring 3                  | Temporarily removed    |
| Emulator                | QEMU                   |
| Physical Testing        | Real x86 hardware      |
| Build System            | GNU Make               |
| License                 | GPL-2.0-only           |

## Kernel Architecture

The kernel is organized into independent subsystems.

The current source tree includes areas for:

```text
kernel/
├── drivers/
├── handlers/
├── idt/
├── interrupts/
├── process/
├── timer/
└── kmain.c
```

The architecture is expected to change as the project evolves.

The long-term direction is to establish clear boundaries between:

```text
CPU
 │
 ├── GDT
 ├── TSS
 └── Privilege Levels
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

## Project Structure

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
│   │
│   ├── handlers/
│   │   ├── handler_0x00.c
│   │   ├── handler_0x08.c
│   │   ├── handler_central.asm
│   │   ├── handler_irq0.c
│   │   ├── interrupt.h
│   │   ├── keyboard_handler.c
│   │   ├── ring_3.asm
│   │   ├── syscall.asm
│   │   └── syscall.c
│   │
│   ├── idt/
│   │   ├── idt.asm
│   │   ├── idt.c
│   │   └── idt.h
│   │
│   ├── interrupts/
│   │   ├── io.h
│   │   ├── pic.c
│   │   └── pic.h
│   │
│   ├── process/
│   │   ├── process.c
│   │   └── process.h
│   │
│   ├── timer/
│   │   ├── pit.c
│   │   └── pit.h
│   │
│   └── kmain.c
│
├── LICENSE
├── linker.ld
├── makefile
└── readme.md
```

The `kernel/` directory contains the NexisK kernel source.

The `boot/` directory contains the separate bootloader implementation.

## Build System

NexisK uses a custom GNU Make build system.

The Makefile automatically:

1. Compiles C kernel sources.
2. Assembles NASM sources.
3. Builds the boot stages.
4. Links the kernel and boot components.
5. Generates the bootable disk image.
6. Creates the bootable ISO.

The build pipeline can be represented as:

```text
C / NASM Sources
       │
       ▼
 Kernel Objects
       │
       ├───────────────┐
       │               │
       ▼               ▼
   Stage 1          Stage 2
       │               │
       └───────┬───────┘
               ▼
            disk.img
               │
               ▼
            boot.img
               │
               ▼
          NexisK.iso
```

The kernel is compiled as a freestanding 32-bit i386 target.

The current compiler configuration includes:

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

## Requirements

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

## Building

Clone the repository:

```bash
git clone https://github.com/icarotelesdasilva/NexisK.git
cd NexisK
```

Build the kernel and bootable ISO:

```bash
make
```

The generated files are placed inside:

```text
build/
```

The main bootable image is:

```text
build/NexisK.iso
```

## Running with QEMU

Run the kernel through the generated ISO:

```bash
make run
```

This starts:

```text
qemu-system-i386
```

with the NexisK ISO attached as a CD-ROM.

Serial output is connected to the terminal, making it useful for development and debugging.

## Development Debug Mode

For deeper QEMU diagnostics:

```bash
make dev
```

This enables additional debugging information and writes the QEMU log to:

```text
build/qemu.log
```

The debug configuration enables diagnostics for areas such as:

* Guest errors
* Unimplemented behavior
* Interrupts
* CPU resets
* MMU activity
* Protected execution events

## Cleaning the Build

To remove all generated build artifacts:

```bash
make clean
```

A clean build can then be produced with:

```bash
make
```

## Boot Process

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
          ┌──────────┼──────────┐
          │          │          │
          ▼          ▼          ▼
      Initialize   Boot Menu   Memory
       Environment  Selection   Detection
                                  │
                                  ▼
                              E820 Map
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
                       ┌──────────┼──────────┐
                       │          │          │
                       ▼          ▼          ▼
                      IDT        PIC        PIT
                       │          │          │
                       └──────────┼──────────┘
                                  │
                                  ▼
                            Kernel Runtime
```

The bootloader is responsible for initializing the machine, detecting the available physical memory map, getting the kernel into memory and transferring control to it.

The kernel then initializes its own runtime subsystems.

## Bootable Image

The build system creates a raw boot image and packages it into an ISO.

The generated files include:

```text
build/
├── stage1.bin
├── stage2.bin
├── disk.img
├── iso/
│   └── boot.img
├── NexisK.iso
└── qemu.log
```

The boot image is currently created with a 1.44 MB disk-image size and then packaged as an El Torito bootable ISO.

## Development Roadmap

### Bootloader

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

### CPU and Privilege Management

* [x] i386 target
* [x] Protected-mode foundation
* [ ] Reimplement GDT
* [ ] Reimplement TSS
* [ ] Reimplement Ring 3
* [ ] Restore kernel/user privilege separation
* [ ] Improve CPU initialization

### Interrupts

* [x] IDT
* [x] CPU exception handlers
* [x] PIC
* [x] PIT
* [x] Keyboard interrupts
* [x] PS/2 mouse interrupts
* [x] System-call interrupt
* [ ] More complete interrupt abstraction
* [ ] Improved interrupt dispatching

### Memory Management

* [x] BIOS E820 memory map detection
* [ ] Reimplement Physical Memory Manager
* [ ] Reimplement Virtual Memory Manager
* [ ] Reimplement paging
* [ ] Physical page allocation
* [ ] Physical page freeing
* [ ] Dynamic page mapping
* [ ] Page fault handling
* [ ] Kernel/user memory permissions
* [ ] Per-process address spaces

### Processes

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

### System Calls

* [x] Basic syscall interface
* [x] `int 0x80` entry
* [ ] Expanded syscall ABI
* [ ] Process-related syscalls
* [ ] Memory-related syscalls
* [ ] File-related syscalls
* [ ] User-space API

### Hardware

* [x] VGA text output
* [x] Serial output
* [x] Keyboard
* [x] PS/2 mouse
* [ ] Additional device drivers
* [ ] Storage drivers
* [ ] LBA support
* [ ] Hardware abstraction improvements

### Storage

* [ ] Disk abstraction
* [ ] Disk driver
* [ ] LBA support
* [ ] Filesystem abstraction
* [ ] Initial filesystem
* [ ] File operations

### Future Architecture

* [ ] More complete user-space support
* [ ] Stable kernel ABI
* [ ] Improved hardware abstraction
* [ ] UEFI boot support
* [ ] x86-64 transition

## Temporarily Removed Subsystems

The following subsystems existed in previous versions of NexisK but were intentionally removed during the bootloader refactor:

* GDT
* TSS
* Ring 3
* Physical Memory Manager
* Virtual Memory Manager
* Paging
* Dynamic page mapping

This is an intentional development step rather than an abandonment of these components.

The current E820 implementation provides physical memory discovery while the full memory-management stack is being rebuilt.

The current priority is to establish a stable bootloader and real-hardware boot foundation before rebuilding the higher-level kernel subsystems.

## Versioning

NexisK uses version numbers to track major development milestones.

### Current Release

**[v0.8.6](https://github.com/icarotelesdasilva/NexisK/releases/tag/v0.8.6)**

### Recent Milestones

| Version | Milestone                                         |
| ------- | ------------------------------------------------- |
| v0.4.0  | Physical Memory Manager                           |
| v0.5.0  | Virtual Memory / Paging / PS/2                    |
| v0.6.0  | GDT / Ring 3 / TSS                                |
| v0.7.0  | Basic System Call Interface                       |
| v0.7.1  | Ring 3 Syscall Validation                         |
| v0.7.3  | Basic VMM Page Mapping and Boot/Build Refactoring |
| v0.8.5  | Bootloader refactor and E820 memory map detection |
| v0.8.6  | Verified E820 memory map reporting in the kernel  |

Historical versions may contain kernel subsystems that are not present in the current implementation.

## Development Philosophy

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
Interrupts
    │
    ▼
Memory Management
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

## Design Goals

The long-term goals of NexisK include:

* Build a maintainable x86 kernel
* Understand the hardware/software boundary
* Implement core kernel subsystems from scratch
* Provide a clean process model
* Implement virtual memory
* Implement multitasking
* Establish a stable system-call interface
* Support user-space execution
* Improve hardware support
* Develop a robust boot architecture
* Eventually explore x86-64

The project prioritizes understanding, experimentation and incremental development over premature complexity.

## Limitations

NexisK is experimental and currently has several limitations.

### Architecture

The current kernel targets 32-bit i386/x86.

### Firmware

The current boot path is based on the traditional BIOS environment.

### Memory Management

The full PMM, VMM and paging implementations are currently being rebuilt after the bootloader refactor.

The current implementation provides BIOS E820 memory map detection and kernel-side reporting of the detected regions.

### Privilege Levels

The previous Ring 3 infrastructure has been temporarily removed and is planned for reimplementation.

### Process Management

Process infrastructure exists in an early stage and is not yet a complete multitasking system.

### Scheduling

A complete scheduler has not yet been implemented.

### Storage

Filesystem and general storage support are not currently available as complete kernel subsystems.

### Hardware Compatibility

Real hardware testing is performed, but compatibility with all x86 hardware is not guaranteed.

### Production Use

NexisK is not production-ready and should be considered an experimental kernel.

## Contributing

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

## Debugging

Low-level kernel development often requires debugging below the level of traditional application development.

Useful areas to inspect include:

* BIOS boot behavior
* Stage 1 execution
* Stage 2 execution
* Kernel loading
* E820 memory map detection
* Protected-mode initialization
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

## License

NexisK is licensed under the GNU General Public License v2.0 only (`GPL-2.0-only`).

See [LICENSE](LICENSE) for the complete license text.

## Author

Developed by `icarotelesdasilva`.

GitHub repository:

https://github.com/icarotelesdasilva/NexisK

## Final Note

NexisK is a kernel built to explore what happens underneath an operating system.

It is intentionally developed close to the hardware, from the boot process and CPU initialization to interrupts, memory discovery, memory management, processes and system calls.

The project is still evolving, and its architecture will continue to change as new kernel subsystems are implemented.

<p align="center">
  <strong>NexisK — An experimental kernel built from the ground up.</strong>
</p>
