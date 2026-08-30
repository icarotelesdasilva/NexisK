
 # Nexis Kernel
 # Copyright (C) 2026 Ícaro Teles da Silva Ribeiro
 # Author: @icarotelesdasilva
 # SPDX-License-Identifier: GPL-2.0-only
 
cmake_minimum_required(VERSION 3.10)

project(kernel LANGUAGES C ASM_NASM)

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
        objcopy
        -O binary
        "$<TARGET_FILE:kernel.elf>"
        "${CMAKE_CURRENT_BINARY_DIR}/kernel.bin"

    DEPENDS
        kernel.elf

    COMMENT "Gerando kernel.bin..."
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

    COMMENT "Calculando setores do kernel..."
)


add_custom_command(
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/boot.bin"

    COMMAND
        nasm
        -f bin
        -I "${CMAKE_CURRENT_BINARY_DIR}/"
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/boot.asm"
        -o "${CMAKE_CURRENT_BINARY_DIR}/boot.bin"

    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/boot/boot.asm"
        "${CMAKE_CURRENT_BINARY_DIR}/kernel_sectors.inc"

    COMMENT "Gerando bootloader..."
)


add_custom_command(
    OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/os-image.bin"

    COMMAND
        cat
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

    COMMENT "os-image.bin..."
)

add_custom_target(os_image ALL
    DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/os-image.bin"
)
add_custom_target(run
    COMMAND
        qemu-system-i386
        -drive
        "format=raw,file=${CMAKE_CURRENT_BINARY_DIR}/os-image.bin,if=floppy"

    DEPENDS
        os_image

    COMMENT "Iniciando QEMU..."
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

    COMMENT "clean build..."
)

