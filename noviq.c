#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer/lexer_interpret.h"

#define LITECODE_VERSION "prealpha-v2.3"
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

#define MAX_IF_NESTING 32

typedef struct {
    char condition[256];
    char content[4096];
    char elseContent[4096];
    int braceCount;
    int hasElse;
} IfBlock;

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
    int lineNumber = 0;
    int inMultilineComment = 0;
    
    // Stack of if blocks
    IfBlock ifStack[MAX_IF_NESTING];
    int ifStackPtr = -1;

    while ((read = getline(&line, &len, file)) != -1) {
        lineNumber++;
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }

        // Skip empty lines or lines with only whitespace
        char *trimmed = line;
        while (*trimmed == ' ' || *trimmed == '\t') trimmed++;
        if (*trimmed == '\0') continue;

        // Handle comments
        if (strncmp(trimmed, "##", 2) == 0) {
            inMultilineComment = !inMultilineComment;
            continue;
        }
        if (inMultilineComment) continue;
        if (strncmp(trimmed, "#", 1) == 0) continue;

        char *commentStart = strstr(trimmed, "#");
        if (commentStart != NULL) {
            *commentStart = '\0';
            if (*trimmed == '\0') continue;
        }

        // Handle if blocks and other commands
        if (ifStackPtr >= 0) {
            // We're inside an if block
            for (char *c = trimmed; *c; c++) {
                if (*c == '{') {
                    ifStack[ifStackPtr].braceCount++;
                }
                else if (*c == '}') {
                    ifStack[ifStackPtr].braceCount--;
                    if (ifStack[ifStackPtr].braceCount == 0) {
                        // Check for else
                        char *elseStart = strstr(c + 1, "else");
                        if (elseStart && elseStart[-1] <= ' ' && elseStart[4] <= ' ') {
                            // Found else, look for its opening brace
                            char *elseBrace = strchr(elseStart, '{');
                            if (!elseBrace) {
                                fprintf(stderr, "Error on line %d: Missing opening brace for else block\n", lineNumber);
                                exit(EXIT_FAILURE);
                            }
                            ifStack[ifStackPtr].hasElse = 1;
                            *c = '\0'; // End if block here
                            break;
                        } else {
                            // No else, execute the if block
                            char *fullCmd = malloc(strlen(ifStack[ifStackPtr].condition) + 
                                                strlen(ifStack[ifStackPtr].content) + 32);
                            sprintf(fullCmd, "if(%s){%s}", 
                                    ifStack[ifStackPtr].condition, 
                                    ifStack[ifStackPtr].content);
                            interpretCommand(fullCmd, lineNumber);
                            free(fullCmd);
                            ifStackPtr--;
                            *c = '\0';
                            break;
                        }
                    }
                }
            }
            
            if (ifStackPtr >= 0) {
                if (ifStack[ifStackPtr].hasElse) {
                    // Accumulating else block content
                    if (ifStack[ifStackPtr].elseContent[0] != '\0') {
                        strcat(ifStack[ifStackPtr].elseContent, " ");
                    }
                    strcat(ifStack[ifStackPtr].elseContent, trimmed);
                    
                    // Check if else block is complete
                    if (strchr(trimmed, '}')) {
                        char *fullCmd = malloc(strlen(ifStack[ifStackPtr].condition) + 
                                            strlen(ifStack[ifStackPtr].content) + 
                                            strlen(ifStack[ifStackPtr].elseContent) + 64);
                        sprintf(fullCmd, "if(%s){%s}else{%s}", 
                                ifStack[ifStackPtr].condition,
                                ifStack[ifStackPtr].content,
                                ifStack[ifStackPtr].elseContent);
                        interpretCommand(fullCmd, lineNumber);
                        free(fullCmd);
                        ifStackPtr--;
                    }
                } else {
                    // Still in if block, append this line
                    if (ifStack[ifStackPtr].content[0] != '\0') {
                        strcat(ifStack[ifStackPtr].content, " ");
                    }
                    strcat(ifStack[ifStackPtr].content, trimmed);
                }
            }
        } else if (strncmp(trimmed, "if(", 3) == 0) {
            // Start new if block
            ifStackPtr++;
            ifStack[ifStackPtr].braceCount = 0;
            ifStack[ifStackPtr].content[0] = '\0';
            ifStack[ifStackPtr].elseContent[0] = '\0';
            ifStack[ifStackPtr].hasElse = 0;
            
            // Extract condition
            char *openParen = strchr(trimmed, '(');
            char *closeParen = strchr(openParen, ')');
            if (!closeParen) {
                fprintf(stderr, "Error on line %d: Missing closing parenthesis\n", lineNumber);
                exit(EXIT_FAILURE);
            }
            
            strncpy(ifStack[ifStackPtr].condition, 
                    openParen + 1, 
                    closeParen - (openParen + 1));
            ifStack[ifStackPtr].condition[closeParen - (openParen + 1)] = '\0';

            // Get content after condition
            char *content = strchr(closeParen, '{');
            if (content) {
                ifStack[ifStackPtr].braceCount = 1;
                strcat(ifStack[ifStackPtr].content, content + 1);
                
                // Check if it's a single-line if
                if (strchr(content, '}')) {
                    char *fullCmd = malloc(strlen(trimmed) + 1);
                    strcpy(fullCmd, trimmed);
                    interpretCommand(fullCmd, lineNumber);
                    free(fullCmd);
                    ifStackPtr--;
                }
            }
        }
        else if (ifStackPtr < 0) {
            // Regular command outside if block
            interpretCommand(trimmed, lineNumber);
        }
    }

    if (ifStackPtr >= 0) {
        fprintf(stderr, "Error: Unclosed if block at end of file\n");
        exit(EXIT_FAILURE);
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