#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char buffer[256];
char search[] = "BUILD_PROJECT";
int found = 0;

int main() {
    FILE *file = fopen("build.nexis", "r"); 

    if (file == NULL) {
        printf("Error: build.nexis not found.\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strstr(buffer, search) != NULL) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found) {
        system("make clean && make all");
    } else {
        printf("Error: Syntax '%s' not found.\n", search);
        return 1;
    }

    return 0;
}

