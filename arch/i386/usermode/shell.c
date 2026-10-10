#include "../kernel/syscall/syscall.h"

#include <stdint.h>

#define BUFFER_SIZE 256

static char command[BUFFER_SIZE];
static int len = 0;

static int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

static void print_string(const char* mensagem) {
    __asm__ __volatile__ (
        "int $0x80"
        :
        : "a"(1),
          "b"((uint32_t)mensagem)
        : "memory"
    );
}

static void clear_buffer(void) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        command[i] = '\0';
    }
    len = 0;
}

static void execute_command(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        print_string("NexisK Commands: help, clear, version\n");
    } else if (strcmp(cmd, "version") == 0) {
        print_string("NexisK OS v0.1.0 (i386 Mode)\n");
    } else if (strcmp(cmd, "clear") == 0) {
        print_string("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    } else if (len > 0) {
        print_string("Error: Command not found.\n");
    }
}

void shell_C(char caract) {
    if (caract == '\n') {
        command[len] = '\0';
        execute_command(command);
        clear_buffer();
    } else if (caract == '\b') {
        if (len > 0) {
            len--;
            command[len] = '\0';
        }
    } else {
        if (len < (BUFFER_SIZE - 1)) {
            command[len] = caract;
            len++;
            command[len] = '\0';
        }
    }
}

