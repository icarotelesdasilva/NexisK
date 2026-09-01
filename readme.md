# NexisK

A general-purpose kernel built from scratch for the **i386 architecture**.

NexisK is an experimental operating system kernel focused on low-level development, x86 protected mode, hardware interaction, interrupt handling, memory management, privilege levels, system calls, virtual memory and process-related mechanisms.

The project is written from scratch using **C and NASM assembly**, with a custom bootloader and no external bootloader dependency.

## Current Status

NexisK currently boots into **i386 protected mode** and provides a functional low-level kernel foundation including:

* Custom BIOS bootloader
* Automatic kernel sector calculation
* Bootable ISO generation
* El Torito BIOS boot support through floppy emulation
* i386 protected mode
* Global Descriptor Table (GDT)
* Task State Segment (TSS)
* Ring 3 support
* Interrupt Descriptor Table (IDT)
* CPU exception handlers
* Hardware interrupt handling
* Programmable Interrupt Controller (PIC)
* Programmable Interval Timer (PIT)
* PS/2 mouse interrupt handling
* VGA text output
* Serial output
* Physical Memory Manager (PMM)
* Virtual Memory Manager (VMM)
* Paging
* Dynamic virtual page mapping
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

The ISO is booted through **BIOS El Torito floppy emulation**. The custom NexisK bootloader remains responsible for loading and starting the kernel.

### Run the Floppy Image

The raw floppy image can also be tested directly:

```bash
make run_floppy
```

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
| Boot media           | ISO / El Torito floppy emulation |
| Memory management    | PMM + VMM                        |
| Paging               | Enabled                          |
| Virtual mapping      | Basic `map_page()`               |
| Privilege levels     | Ring 0 / Ring 3                  |
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
* Obtains the BIOS E820 memory map
* Loads the kernel from the boot image
* Loads the GDT
* Enters i386 protected mode
* Initializes the protected-mode kernel stack
* Transfers execution to the kernel

The build system calculates the number of 512-byte sectors required by the kernel automatically.

The generated boot image contains:

```text
sector 0       bootloader
sector 1..N    kernel
remaining      padding
```

The boot image is currently packaged into a 1.44 MB floppy image and exposed to BIOS through El Torito floppy emulation when building the final ISO.

The current bootloader is intentionally simple and remains based on BIOS CHS disk access. A future refactor is planned to remove the floppy/CHS assumptions and use a more general bare-metal disk-loading mechanism.

### Bootable ISO

The build system generates a bootable ISO:

```text
NexisK.iso
```

The ISO contains the generated 1.44 MB boot image as its El Torito BIOS boot image.

The current boot chain is:

```text
BIOS
  |
  v
El Torito
  |
  v
Floppy-emulated boot image
  |
  v
Custom NexisK bootloader
  |
  v
Kernel
```

The ISO generation is handled as part of the CMake build system.

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

The TSS provides kernel stack information required during transitions from user mode back into Ring 0.

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

NexisK currently contains two layers of memory management.

#### Physical Memory Manager

The PMM uses the BIOS E820 memory map provided during boot to identify physical memory regions.

Current functionality includes:

* Physical page discovery
* Bitmap-based page tracking
* Physical page allocation
* Physical page freeing
* Bitmap protection

Example allocation behavior:

```text
Page 1: 0x00104000
Page 2: 0x00105000
Page 3: 0x00104000
```

The third allocation can reuse a page previously freed by the allocator.

#### Virtual Memory Manager

The VMM currently provides the paging foundation and basic dynamic page mapping.

Current implementation includes:

* Page directory creation
* Page table creation
* Initial identity mapping
* CR3 initialization
* Paging activation through CR0
* Basic virtual-to-physical page mapping
* TLB invalidation with `invlpg`

The current `map_page()` implementation allows mappings such as:

```text
virtual 0x00400000
        |
        v
physical page allocated by PMM
```

The mapping is currently created with present and writable permissions.

The VMM is still being expanded toward a complete virtual memory subsystem.

### Context Switching

NexisK contains initial process and context-switching infrastructure.

The context-switch mechanism preserves processor execution state so execution can later continue from a saved context.

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
* [x] Bootable ISO generation
* [x] El Torito BIOS boot support
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
* [x] Basic dynamic page mapping

### Planned

* [ ] Expand virtual memory management
* [ ] `unmap_page()`
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
* [ ] Replace floppy/CHS assumptions in the bootloader
* [ ] General LBA disk loading
* [ ] More robust kernel loading
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
```

## Project Goals

The long-term goal of NexisK is to evolve from an experimental i386 kernel into a more complete operating system kernel while keeping the implementation understandable and developed from the lowest levels upward.

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
