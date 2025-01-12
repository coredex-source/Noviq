#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexar/lexar_interpret.h"

// Function to read and execute commands from a file
void executeFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int lineNumber = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        lineNumber++;
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        interpretCommand(line, lineNumber);
    }

    free(line);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-e") != 0) {
        fprintf(stderr, "Usage: %s -e filename.lc\n", argv[0]);
        return 1;
    }

    const char *filename = argv[2];
    executeFile(filename);

    return 0;
}