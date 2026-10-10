![NexisK](media/NexisK.PNG)


# NexisK

**NexisK — A general-purpose kernel built from scratch for the i386 architecture.**

NexisK is a 32-bit x86 kernel project focused on building an operating system from the ground up for the **i386 architecture**.

The project is currently focused on **version 0.1**, where the kernel is being reworked into a more modular architecture and the foundations required for userspace execution are being introduced.

## NexisK 0.1

Version 0.1 represents a major **kernel refactor**.

The previous kernel structure was reorganized to separate CPU initialization, interrupt handling, memory management, privilege management, system calls and drivers into independent subsystems.

This refactor provides a cleaner foundation for further development instead of continuing to expand a monolithic kernel structure.

### Added in 0.1

- Kernel source tree refactor
- GDT subsystem
- IDT subsystem
- ISR infrastructure
- Exception handling
- PIC subsystem
- Physical Memory Manager (PMM)
- Virtual Memory Manager (VMM)
- TLB management
- TSS support
- System call infrastructure
- VGA driver
- Keyboard driver
- Kernel panic handling
- Low-level I/O helpers
- Initial userspace test programs

The **userspace layer is currently experimental** and is being developed as part of the 0.1 foundation.

## Architecture

NexisK targets the **i386 / x86 32-bit architecture**.

The architecture is intentionally organized into separate subsystems:




kernel/
├── drivers/
├── GDT/
├── IDT/
├── inlines/
├── interrupts/
├── memory/
├── syscall/
├── TLB/
├── TSS/
└── VMM/

