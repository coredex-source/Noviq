#ifndef ERROR_H
#define ERROR_H

/**
 * @file error.h
 * @brief Error handling system for Noviq interpreter
 */

typedef enum {
    ERROR_SYNTAX,
    ERROR_RUNTIME,
    ERROR_TYPE,
    ERROR_UNDEFINED_VAR,
    ERROR_CONST_MODIFY,
    ERROR_DIV_BY_ZERO,
    ERROR_FILE,
    ERROR_MEMORY
} ErrorType;

/**
 * @brief Report an error and exit the program
 * @param type The type of error
 * @param lineNumber The line number where the error occurred (0 if not applicable)
 * @param format The error message format string
 * @param ... Variable arguments for the format string
 */
void reportError(ErrorType type, int lineNumber, const char *format, ...);

/**
 * @brief Set the current file context for error reporting
 * @param filename The name of the file being processed
 */
void setErrorFile(const char *filename);

/**
 * @brief Get the current line number
 * @return The current line number
 */
int getCurrentLine(void);

/**
 * @brief Set the current line number
 * @param lineNumber The line number to set
 */
void setCurrentLine(int lineNumber);

#endif // ERROR_H
