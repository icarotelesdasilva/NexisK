cmake_minimum_required(VERSION 3.10)
project(kernel LANGUAGES C ASM_NASM)

set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -f elf32 <FLAGS> -o <OBJECT> <SOURCE>")

set(CMAKE_C_FLAGS "-m32 -ffreestanding -fno-pie -fno-stack-protector -fno-asynchronous-unwind-tables -fno-unwind-tables -nostdlib")

file(GLOB_RECURSE KERNEL_SOURCES 
    "kernel/*.c"
    "kernel/*.asm"
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/boot.bin
    COMMAND nasm -f bin ${CMAKE_CURRENT_SOURCE_DIR}/boot/boot.asm -o ${CMAKE_CURRENT_BINARY_DIR}/boot.bin
    DEPENDS boot/boot.asm
    COMMENT " bootloader..."
)

add_executable(kernel.elf ${KERNEL_SOURCES})
target_link_options(kernel.elf PRIVATE 
    "-no-pie"
    "-m32"
    "-Wl,-m,elf_i386" 
    "-Wl,-T,${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
    "-Wl,--no-warn-rwx-segments"
    "-nostdlib"
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/os-image.bin
    COMMAND objcopy -O binary ${CMAKE_CURRENT_BINARY_DIR}/kernel.elf ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin
    COMMAND cat ${CMAKE_CURRENT_BINARY_DIR}/boot.bin ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin > ${CMAKE_CURRENT_BINARY_DIR}/os-image.bin
    COMMAND truncate -s 1474560 ${CMAKE_CURRENT_BINARY_DIR}/os-image.bin
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/boot.bin kernel.elf
    COMMENT "os-image.bin..."
)

add_custom_target(os_image ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/os-image.bin)

add_custom_target(run
    COMMAND qemu-system-i386 -drive format=raw,file=${CMAKE_CURRENT_BINARY_DIR}/os-image.bin,if=floppy
    DEPENDS os_image
    COMMENT "QEMU..."
)
