# Nexis Kernel
# Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
# Author: @icarotelesdasilva
# SPDX-License-Identifier: GPL-2.0-only


cmake_minimum_required(VERSION 3.10)

project(kernel LANGUAGES C ASM_NASM)



find_program(GENISOIMAGE genisoimage REQUIRED)
find_program(OBJCOPY objcopy REQUIRED)
find_program(NASM nasm REQUIRED)



set(CMAKE_ASM_NASM_COMPILE_OBJECT
    "<CMAKE_ASM_NASM_COMPILER> -f elf32 <FLAGS> -o <OBJECT> <SOURCE>"
)

set(CMAKE_C_FLAGS
    "-m32 -ffreestanding -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -fno-unwind-tables -nostdlib"
)



file(GLOB_RECURSE KERNEL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/kernel/*.asm"
)

add_executable(kernel.elf ${KERNEL_SOURCES})


set_target_properties(kernel.elf PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)


target_link_options(kernel.elf PRIVATE
    "-no-pie"
    "-m32"
    "-Wl,-m,elf_i386"
    "-Wl,-T,${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
    "-Wl,--no-warn-rwx-segments"
    "-nostdlib"
)



add_custom_command(
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/kernel.bin"

    COMMAND
        ${OBJCOPY}
        -O binary
        "$<TARGET_FILE:kernel.elf>"
        "${CMAKE_CURRENT_BINARY_DIR}/kernel.bin"

    DEPENDS
        kernel.elf

    COMMENT "Generating kernel.bin..."
)



add_custom_command(
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/kernel_sectors.inc"

    COMMAND
        ${CMAKE_COMMAND}
        -D KERNEL_BIN="${CMAKE_CURRENT_BINARY_DIR}/kernel.bin"
        -D OUTPUT_FILE="${CMAKE_CURRENT_BINARY_DIR}/kernel_sectors.inc"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/calculate_sectors.cmake"

    DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/kernel.bin"
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/calculate_sectors.cmake"

    COMMENT "Calculating kernel sectors..."
)


add_custom_command(
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/boot.bin"

    COMMAND
        ${NASM}
        -f bin
        -I "${CMAKE_CURRENT_BINARY_DIR}/"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/boot.asm"
        -o "${CMAKE_CURRENT_BINARY_DIR}/boot.bin"

    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/boot.asm"
        "${CMAKE_CURRENT_BINARY_DIR}/kernel_sectors.inc"

    COMMENT "Generating bootloader..."
)



add_custom_command(
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/os-image.bin"

    COMMAND
        ${CMAKE_COMMAND} -E cat
        "${CMAKE_CURRENT_BINARY_DIR}/boot.bin"
        "${CMAKE_CURRENT_BINARY_DIR}/kernel.bin"
        >
        "${CMAKE_CURRENT_BINARY_DIR}/os-image.bin"

    COMMAND
        truncate
        -s 1474560
        "${CMAKE_CURRENT_BINARY_DIR}/os-image.bin"

    DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/boot.bin"
        "${CMAKE_CURRENT_BINARY_DIR}/kernel.bin"

    COMMENT "Generating floppy boot image..."
)



set(ISO_ROOT
    "${CMAKE_CURRENT_BINARY_DIR}/iso-root"
)

add_custom_command(
    OUTPUT
        "${ISO_ROOT}/os-image.bin"

    COMMAND
        ${CMAKE_COMMAND} -E make_directory
        "${ISO_ROOT}"

    COMMAND
        ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_BINARY_DIR}/os-image.bin"
        "${ISO_ROOT}/os-image.bin"

    DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/os-image.bin"

    COMMENT "Preparing ISO boot image..."
)



set(ISO_IMAGE
    "${CMAKE_CURRENT_BINARY_DIR}/NexisK.iso"
)

add_custom_command(
    OUTPUT
        "${ISO_IMAGE}"

    COMMAND
        ${GENISOIMAGE}
        -R
        -J
        -V NEXISK
        -b os-image.bin
        -c boot.catalog
        -o "${ISO_IMAGE}"
        "${ISO_ROOT}"

    DEPENDS
        "${ISO_ROOT}/os-image.bin"

    COMMENT "Generating NexisK.iso..."
)



add_custom_target(os_image ALL
    DEPENDS
        "${ISO_IMAGE}"
)


add_custom_target(run
    COMMAND
        qemu-system-i386
        -cdrom
        "${ISO_IMAGE}"
        -boot d
        -serial stdio

    DEPENDS
        os_image

    COMMENT "Starting NexisK ISO..."
)



add_custom_target(run_floppy
    COMMAND
        qemu-system-i386
        -drive
        "format=raw,file=${CMAKE_CURRENT_BINARY_DIR}/os-image.bin,if=floppy"

    DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/os-image.bin"

    COMMENT "Starting NexisK floppy image..."
)


add_custom_target(distclean
    COMMAND
        ${CMAKE_COMMAND} -E remove_directory
        "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles"

    COMMAND
        ${CMAKE_COMMAND} -E remove
        "${CMAKE_CURRENT_BINARY_DIR}/CMakeCache.txt"

    COMMAND
        ${CMAKE_COMMAND} -E remove
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake"

    COMMAND
        ${CMAKE_COMMAND} -E remove
        "${CMAKE_CURRENT_BINARY_DIR}/Makefile"

    COMMAND
        ${CMAKE_COMMAND} -E remove
        "${CMAKE_CURRENT_BINARY_DIR}/boot.bin"

    COMMAND
        ${CMAKE_COMMAND} -E remove
        "${CMAKE_CURRENT_BINARY_DIR}/kernel.bin"

    COMMAND
        ${CMAKE_COMMAND} -E remove
        "${CMAKE_CURRENT_BINARY_DIR}/kernel.elf"

    COMMAND
        ${CMAKE_COMMAND} -E remove
        "${CMAKE_CURRENT_BINARY_DIR}/kernel_sectors.inc"

    COMMAND
        ${CMAKE_COMMAND} -E remove
        "${CMAKE_CURRENT_BINARY_DIR}/os-image.bin"

    COMMAND
        ${CMAKE_COMMAND} -E remove
        "${CMAKE_CURRENT_BINARY_DIR}/NexisK.iso"

    COMMAND
        ${CMAKE_COMMAND} -E remove_directory
        "${CMAKE_CURRENT_BINARY_DIR}/iso-root"

    COMMENT "Cleaning build..."
)