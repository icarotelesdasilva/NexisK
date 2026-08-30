if(NOT DEFINED KERNEL_BIN)
    message(FATAL_ERROR "KERNEL_BIN não definido")
endif()

if(NOT DEFINED OUTPUT_FILE)
    message(FATAL_ERROR "OUTPUT_FILE não definido")
endif()

file(SIZE "${KERNEL_BIN}" KERNEL_SIZE)

math(EXPR KERNEL_SECTORS
    "(${KERNEL_SIZE} + 511) / 512"
)

if(KERNEL_SECTORS LESS 1)
    set(KERNEL_SECTORS 1)
endif()

file(WRITE "${OUTPUT_FILE}"
"KERNEL_SECTORS equ ${KERNEL_SECTORS}\n"
)

message(STATUS
    "kernel.bin: ${KERNEL_SIZE} bytes -> ${KERNEL_SECTORS} setores"
)
