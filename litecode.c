#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexar/lexar_interpret.h"

#ifdef _WIN32
// Windows implementation of getline
ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        size = 128;
        bufptr = (char *)malloc(size);
        if (bufptr == NULL) {
            return -1;
        }
    }
    p = bufptr;
    while (c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
            p = bufptr + (p - bufptr);
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}
#endif

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
        
        // Skip empty lines or lines with only whitespace
        char *trimmed = line;
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
        if (*trimmed == '\0') continue;

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