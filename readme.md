# NexisK

A general-purpose kernel built from scratch for the **i386 architecture**.

NexisK is an experimental operating system kernel focused on low-level development, x86 architecture, hardware interaction, interrupt handling, bootloader development, system calls and process-related mechanisms.

The project is written from scratch using **C and NASM assembly**, with a custom bootloader and no external bootloader dependency.

## Current Status

NexisK currently boots through its custom bootloader and provides a low-level kernel foundation including:

* Custom BIOS bootloader
* Automatic kernel sector calculation
* Bootable image generation
* Real hardware boot support
* Bootloader menu
* Custom BIOS bootloader
* Interrupt Descriptor Table (IDT)
* CPU exception handlers
* Hardware interrupt handling
* Programmable Interrupt Controller (PIC)
* Programmable Interval Timer (PIT)
* PS/2 mouse interrupt handling
* VGA text output
* Serial output
* Basic context-switching infrastructure
* Basic system call interface

The project is still under active development and should be considered experimental.

## Quick Start

### Requirements

* Linux
* GCC
* NASM
* CMake
* QEMU
* `genisoimage`

### Build

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

The build system generates:

```text
kernel.elf
kernel.bin
kernel_sectors.inc
boot.bin
os-image.bin
NexisK.iso
```

### Run

Run the bootable ISO with:

```bash
make run
```

The custom NexisK bootloader is responsible for initializing the machine, presenting the boot menu and loading the selected kernel. The bootloader has been validated on real hardware.

### Run the Boot Image

The generated boot image can also be tested directly according to the target hardware/emulator configuration.

### Clean Build

```bash
make distclean
mkdir build
cd build
cmake ..
make -j$(nproc)
```

## Architecture

| Component            | Current implementation           |
| -------------------- | -------------------------------- |
| Architecture         | i386                             |
| CPU mode             | Protected Mode                   |
| Language             | C / NASM                         |
| Boot                 | Custom BIOS bootloader           |
| Boot media           | Boot image / real hardware      |
| Memory management    | Temporarily removed             |
| Paging               | Temporarily removed             |
| Virtual mapping      | Temporarily removed             |
| Privilege levels     | Temporarily removed             |
| Interrupt controller | PIC                              |
| Timer                | PIT                              |
| Emulator             | QEMU                             |
| Build system         | CMake                            |
| License              | GPL-2.0-only                     |

## Features

### Bootloader

NexisK uses a custom x86 BIOS bootloader instead of relying on GRUB, Limine or another external bootloader.

The bootloader currently:

* Initializes the real-mode environment
* Initializes the boot environment
* Presents the boot menu
* Selects the target kernel
* Loads the selected kernel
* Transfers execution to the kernel

The build system calculates the number of 512-byte sectors required by the kernel automatically.

The generated boot image contains the bootloader and the kernel data required by the current boot process.

The current bootloader is no longer treated as a floppy-only boot path. It has been tested on real hardware and includes a boot menu for selecting the available system/kernel entries.

The bootloader is under active development. Its current priority is reliable bare-metal hardware boot and kernel selection through the boot menu.

### Bootable ISO

The build system generates a bootable ISO:

```text
NexisK.iso
```

The boot chain is:

```text
BIOS / real hardware
  |
  v
Custom NexisK bootloader
  |
  v
Boot menu
  |
  +--> Kernel / system entry 1
  |
  +--> Kernel / system entry 2
```

The ISO generation is handled as part of the CMake build system.

### Memory Management and GDT

The previous PMM, VMM, paging, GDT, TSS and Ring 3 infrastructure has been completely removed from the current implementation during the bootloader refactor.

These subsystems are **not part of the current stable implementation** and are planned to be reintroduced in the next update after the new bootloader architecture is stabilized.


### Interrupts

The kernel contains an Interrupt Descriptor Table and handlers for processor exceptions and hardware interrupts.

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

The current implementation contains a basic syscall used to validate the Ring 3 → Ring 0 transition and return path.

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

Memory management has been **completely removed from the current implementation**.

The previous PMM, VMM and paging code was removed during the bootloader refactor. This is intentional: the project is currently prioritizing a stable real-hardware boot path and bootloader architecture.

The following components are planned for reimplementation in the next update:

* Physical Memory Manager (PMM)
* Virtual Memory Manager (VMM)
* Paging
* Dynamic virtual page mapping
* Page fault handling
* User/kernel memory permissions

### Context Switching

NexisK contains initial process-related infrastructure.

The process and context-switching subsystem remains under development and will be rebuilt alongside the memory and privilege-level infrastructure in future updates.

## Kernel Structure

```text
boot
│   ├── stage1.S
│   └── stage2.S
├── kernel
│   ├── drivers
│   │   ├── kernel_panic.c
│   │   ├── mouse.c
│   │   ├── serial.asm
│   │   ├── serial_print.c
│   │   └── vga.c
│   ├── handlers
│   │   ├── handler_0x00.c
│   │   ├── handler_0x08.c
│   │   ├── handler_central.asm
│   │   ├── handler_irq0.c
│   │   ├── interrupt.h
│   │   ├── keyboard_handler.c
│   │   ├── ring_3.asm
│   │   ├── syscall.asm
│   │   └── syscall.c
│   ├── idt
│   │   ├── idt.asm
│   │   ├── idt.c
│   │   └── idt.h
│   ├── interrupts
│   │   ├── io.h
│   │   ├── pic.c
│   │   └── pic.h
│   ├── kmain.c
│   ├── process
│   │   ├── process.c
│   │   └── process.h
│   └── timer
│       ├── pit.c
│       └── pit.h
├── LICENSE
├── linker.ld
├── makefile
└── readme.md


```

## Build System

The build pipeline is:

```text
C / NASM sources
        |
        v
    kernel.elf
        |
        v
    kernel.bin
        |
        v
calculate_sectors.cmake
        |
        v
kernel_sectors.inc
        |
        v
     boot.bin
        |
        v
boot.bin + kernel.bin
        |
        v
   os-image.bin
        |
        v
    genisoimage
        |
        v
     NexisK.iso
```

The raw floppy image and ISO are generated automatically by CMake.

## Boot Process

The current boot process is approximately:

```text
BIOS
 |
 v
Custom Bootloader
 |
 +--> Initialize Real Mode
 |
 +--> Read E820 Memory Map
 |
 +--> Load Kernel
 |
 +--> Load GDT
 |
 +--> Enable Protected Mode
 |
 v
init_pm
 |
 +--> Load Protected-Mode Segments
 |
 +--> Initialize Kernel Stack
 |
 v
Kernel Entry
 |
 v
kmain
 |
 +--> GDT / TSS
 |
 +--> PIC
 |
 +--> PMM
 |
 +--> VMM / Paging
 |
 +--> IDT
 |
 +--> PIT
 |
 +--> PS/2
 |
 +--> Syscall / Ring 3
 |
 +--> Kernel initialization
```

## Development Roadmap

### Completed

* [x] Custom BIOS bootloader
* [x] Automatic kernel sector calculation
* [x] Kernel loading
* [x] Boot menu
* [x] Real hardware boot validation
* [x] Bootable image generation
* [x] IDT
* [x] CPU exception handlers
* [x] PIC
* [x] PIT
* [x] VGA output
* [x] Serial output
* [x] Keyboard interrupt handling
* [x] PS/2 mouse interrupt handling
* [x] Basic syscall interface
* [x] Initial process/context infrastructure

### Temporarily Removed

* [ ] GDT
* [ ] TSS
* [ ] Ring 3
* [ ] Physical Memory Manager (PMM)
* [ ] Virtual Memory Manager (VMM)
* [ ] Paging
* [ ] Dynamic page mapping

### Planned

* [ ] Reimplement GDT
* [ ] Reimplement TSS and privilege levels
* [ ] Reimplement Physical Memory Manager
* [ ] Reimplement Virtual Memory Manager
* [ ] Reimplement paging
* [ ] Reimplement dynamic page mapping
* [ ] Page fault handling
* [ ] User/kernel page permission separation
* [ ] Per-process address spaces
* [ ] Process creation
* [ ] PID management
* [ ] Scheduler
* [ ] Full multitasking
* [ ] More system calls
* [ ] User-space programs
* [ ] Filesystem
* [ ] Storage drivers
* [ ] UEFI boot support
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
v0.7.3  Basic VMM Page Mapping + Boot/Build Refactoring
v0.8.0  Bootloader Refactor / Real Hardware Boot / Boot Menu
```

## Project Goals

The long-term goal of NexisK is to evolve from an experimental i386 kernel into a more complete operating system kernel while keeping the implementation understandable and developed from the lowest levels upward. The current development priority is a robust real-hardware bootloader and boot menu before reintroducing the removed kernel subsystems.

The project is primarily a learning and experimentation project focused on understanding:

* x86 architecture
* CPU privilege levels
* Interrupts
* Physical memory management
* Virtual memory and paging
* Processes
* Context switching
* System calls
* Kernel/user-space boundaries
* Operating system internals
* Bootloader development
* Hardware interaction

## License

NexisK is licensed under the **GNU General Public License v2.0 only**.

See [`LICENSE`](LICENSE) for the complete license text.
