#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static const char *currentFile = NULL;
static int currentLine = 0;

void setErrorFile(const char *filename) {
    currentFile = filename;
}

int getCurrentLine(void) {
    return currentLine;
}

void setCurrentLine(int lineNumber) {
    currentLine = lineNumber;
}

void reportError(ErrorType type, int lineNumber, const char *format, ...) {
    const char *errorPrefix[] = {
        "Syntax Error",
        "Runtime Error",
        "Type Error",
        "Undefined Variable Error",
        "Constant Modification Error",
        "Division by Zero Error",
        "File Error",
        "Memory Error"
    };
    
    fprintf(stderr, "%s", errorPrefix[type]);
    
    if (lineNumber > 0) {
        fprintf(stderr, " on line %d", lineNumber);
    }
    
    if (currentFile) {
        fprintf(stderr, " in file '%s'", currentFile);
    }
    
    fprintf(stderr, ": ");
    
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}
