#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer/lexer_interpret.h"

#define LITECODE_VERSION "prealpha-v1.7"

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

void displayHelp(const char *programName) {
    printf("Noviq Interpreter\n");
    printf("Usage: %s [options] or %s -e <filename>\n\n", programName, programName);
    printf("Options:\n");
    printf("  -e <filename>    Execute a Noviq script file\n");
    printf("  --help          Display this help message\n");
    printf("  --version       Display Noviq version\n");
}

// Function to read and execute commands from a file
void executeFile(const char *filename) {
    // Check file extension
    const char *dot = strrchr(filename, '.');
    if (!dot || strcmp(dot, ".nvq") != 0) {
        fprintf(stderr, "Error: File must have .nvq extension\n");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    currentLineNumber = 1;  // Start at line 1
    int inMultilineComment = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        
        // Skip empty lines or lines with only whitespace
        char *trimmed = line;
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
        if (*trimmed == '\0') {
            currentLineNumber++;
            continue;
        }

        // Handle comments without changing line number
        if (strncmp(trimmed, "##", 2) == 0) {
            inMultilineComment = !inMultilineComment;
            currentLineNumber++;
            continue;
        }
        
        if (inMultilineComment || strncmp(trimmed, "#", 1) == 0) {
            currentLineNumber++;
            continue;
        }

        // Process single-line comments at end of line
        char *commentStart = strstr(trimmed, "#");
        if (commentStart != NULL) {
            *commentStart = '\0';
            while (commentStart > trimmed && (*(commentStart-1) == ' ' || *(commentStart-1) == '\t')) {
                commentStart--;
            }
            *commentStart = '\0';
            if (*trimmed == '\0') {
                currentLineNumber++;
                continue;
            }
        }

        // Only interpret non-empty lines that aren't comments
        if (*trimmed) {
            if (strncmp(trimmed, "if(", 3) == 0 || 
                strncmp(trimmed, "elseif(", 7) == 0 || 
                strcmp(trimmed, "else:") == 0) {
                handleIfStatement(trimmed, file);
            } else {
                interpretCommand(trimmed, currentLineNumber);
            }
            // Let handleIfStatement and interpretCommand manage line numbers
        }
        currentLineNumber++;
    }

    free(line);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        displayHelp(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0) {
        displayHelp(argv[0]);
        return 0;
    }

    if (strcmp(argv[1], "--version") == 0) {
        printf("%s\n", LITECODE_VERSION);
        return 0;
    }

    if (strcmp(argv[1], "-e") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Error: No input file specified\n");
            displayHelp(argv[0]);
            return 1;
        }
        const char *filename = argv[2];
        executeFile(filename);
        return 0;
    }

    fprintf(stderr, "Error: Invalid argument '%s'\n", argv[1]);
    displayHelp(argv[0]);
    return 1;
}