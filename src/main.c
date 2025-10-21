#include "config.h"
#include "interpreter.h"
#include "error.h"
#include "variables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

// Windows implementation of getline
#ifdef _WIN32
ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL || stream == NULL || n == NULL) {
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

typedef struct {
    char condition[NOVIQ_MAX_VAR_NAME_LENGTH];
    char content[NOVIQ_MAX_LINE_LENGTH];
    char elseContent[NOVIQ_MAX_LINE_LENGTH];
    int braceCount;
    int hasElse;
} IfBlock;

void displayHelp(const char *programName) {
    printf("Noviq Interpreter %s\n", NOVIQ_VERSION_FULL);
    printf("Usage: %s [options] or %s -e <filename>\n\n", programName, programName);
    printf("Options:\n");
    printf("  -e <filename>      Execute a Noviq script file\n");
    printf("  -h, --h, --help    Display this help message\n");
    printf("  -v, --v, --version Display Noviq version\n");
}

void executeFile(const char *filename) {
    // Check file extension
    const char *dot = strrchr(filename, '.');
    if (!dot || strcmp(dot, NOVIQ_FILE_EXTENSION) != 0) {
        fprintf(stderr, "Error: File must have %s extension\n", NOVIQ_FILE_EXTENSION);
        exit(EXIT_FAILURE);
    }

    // Check if file exists
    if (access(filename, F_OK) != 0) {
        reportError(ERROR_FILE, 0, "File '%s' not found", filename);
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        reportError(ERROR_FILE, 0, "Cannot open file '%s'", filename);
    }

    setErrorFile(filename);

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int lineNumber = 0;
    int inMultilineComment = 0;
    
    // Stack of if blocks
    IfBlock ifStack[NOVIQ_MAX_IF_NESTING];
    int ifStackPtr = -1;

    while ((read = getline(&line, &len, file)) != -1) {
        lineNumber++;
        setCurrentLine(lineNumber);

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
                                reportError(ERROR_SYNTAX, lineNumber, "Missing opening brace for else block");
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
                            interpretCommand(fullCmd);
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
                        interpretCommand(fullCmd);
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
            if (ifStackPtr >= NOVIQ_MAX_IF_NESTING) {
                reportError(ERROR_RUNTIME, lineNumber, 
                           "Maximum if-statement nesting depth (%d) exceeded", NOVIQ_MAX_IF_NESTING);
            }

            ifStack[ifStackPtr].braceCount = 0;
            ifStack[ifStackPtr].content[0] = '\0';
            ifStack[ifStackPtr].elseContent[0] = '\0';
            ifStack[ifStackPtr].hasElse = 0;
            
            // Extract condition - need to handle nested parentheses and strings
            char *openParen = strchr(trimmed, '(');
            if (!openParen) {
                reportError(ERROR_SYNTAX, lineNumber, "Missing opening parenthesis");
            }
            
            // Find the matching closing parenthesis
            char *closeParen = NULL;
            int parenCount = 1;
            int inString = 0;
            char stringChar = 0;
            char *ptr = openParen + 1;
            
            while (*ptr && parenCount > 0) {
                // Handle string literals
                if ((*ptr == '"' || *ptr == '\'') && !inString) {
                    inString = 1;
                    stringChar = *ptr;
                } else if (inString && *ptr == stringChar) {
                    inString = 0;
                }
                
                // Only count parentheses outside of strings
                if (!inString) {
                    if (*ptr == '(') {
                        parenCount++;
                    } else if (*ptr == ')') {
                        parenCount--;
                        if (parenCount == 0) {
                            closeParen = ptr;
                            break;
                        }
                    }
                }
                ptr++;
            }
            
            if (!closeParen) {
                reportError(ERROR_SYNTAX, lineNumber, "Missing closing parenthesis");
            }
            
            strncpy(ifStack[ifStackPtr].condition, 
                    openParen + 1, 
                    closeParen - (openParen + 1));
            ifStack[ifStackPtr].condition[closeParen - (openParen + 1)] = '\0';

            // Get content after condition
            char *content = strchr(closeParen, '{');
            if (content) {
                ifStack[ifStackPtr].braceCount = 1;
                
                // Extract content after opening brace, trimming whitespace
                char *contentStart = content + 1;
                while (*contentStart == ' ' || *contentStart == '\t') contentStart++;
                
                // Check if the if statement is complete on this line
                // Count remaining braces to see if it closes
                int braceCount = 1;
                char *ptr = content + 1;
                while (*ptr) {
                    if (*ptr == '{') braceCount++;
                    else if (*ptr == '}') {
                        braceCount--;
                        if (braceCount == 0) {
                            // Found the matching closing brace - single line if
                            char *fullCmd = malloc(strlen(trimmed) + 1);
                            strcpy(fullCmd, trimmed);
                            interpretCommand(fullCmd);
                            free(fullCmd);
                            ifStackPtr--;
                            break;
                        }
                    }
                    ptr++;
                }
                
                // If we didn't find the closing brace, it's a multi-line if
                if (ifStackPtr >= 0 && braceCount > 0) {
                    // Don't store the opening brace line content if it's just whitespace
                    if (*contentStart != '\0') {
                        strcat(ifStack[ifStackPtr].content, contentStart);
                    }
                }
            }
        }
        else if (ifStackPtr < 0) {
            // Regular command outside if block
            interpretCommand(trimmed);
        }
    }

    if (ifStackPtr >= 0) {
        reportError(ERROR_SYNTAX, 0, "Unclosed if block at end of file");
    }

    free(line);
    fclose(file);
    
    // Cleanup
    freeAllVariables();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        displayHelp(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--h") == 0) {
        displayHelp(argv[0]);
        return 0;
    }

    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--v") == 0) {
        printf("%s\n", NOVIQ_VERSION_FULL);
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
