#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer_display.h"
#include "lexer_interpret.h"

// Function to display text
void display(const char *text) {
    printf("%s\n", text);
}

// Function to display integer
void displayInt(int value) {
    printf("%d\n", value);
}

// Add this function
void displayFloat(float value) {
    printf("%f\n", value);
}

// Add this helper function before displayFormatted
int isExpression(const char* str) {
    for (int i = 0; str[i]; i++) {
        // Check for two-character operators
        if (str[i] == '*' && str[i+1] == '*') return 1;  // **
        if (str[i] == '/' && str[i+1] == '/') return 1;  // //
        // Check for single character operators
        if (strchr("+-*/%", str[i])) return 1;
    }
    return 0;
}

void displayFormatted(const char *format, char **vars, int varCount) {
    char output[1024] = "";
    
    while (*format) {
        if (*format == '%' && strncmp(format, "%var", 4) == 0) {
            format += 4;
            int varNum = 0;
            while (*format >= '0' && *format <= '9') {
                varNum = varNum * 10 + (*format - '0');
                format++;
            }
            
            if (varNum < 1 || varNum > varCount) {
                fprintf(stderr, "Error on line %d: Invalid variable number %d\n", currentLineNumber, varNum);
                exit(EXIT_FAILURE);
            }

            // Get the variable or expression
            char *expr = vars[varNum - 1];
            while (*expr == ' ') expr++; // Skip leading spaces

            // Use isExpression to check for all operator types
            if (isExpression(expr)) {
                // Handle expression
                Variable *result = evaluateExpression(expr);
                if (result) {
                    char numStr[32];
                    if (result->type == INT) {
                        sprintf(numStr, "%d", result->value.intValue);
                    } else {
                        sprintf(numStr, "%.6f", result->value.floatValue);
                    }
                    strcat(output, numStr);
                    free(result);
                } else {
                    fprintf(stderr, "Error on line %d: Invalid expression '%s'\n", currentLineNumber, expr);
                    exit(EXIT_FAILURE);
                }
            } else {
                // Handle simple variable
                Variable *var = findVariable(expr);
                if (!var) {
                    fprintf(stderr, "Error on line %d: Variable '%s' not found\n", currentLineNumber, expr);
                    exit(EXIT_FAILURE);
                }

                char numStr[32];
                switch (var->type) {
                    case INT:
                        sprintf(numStr, "%d", var->value.intValue);
                        strcat(output, numStr);
                        break;
                    case FLOAT:
                        sprintf(numStr, "%.6f", var->value.floatValue);
                        strcat(output, numStr);
                        break;
                    case BOOLEAN:
                        strcat(output, var->value.boolValue ? "true" : "false");
                        break;
                    case STRING:
                        strcat(output, var->value.stringValue);
                        break;
                }
            }
            continue;
        }
        
        int len = strlen(output);
        output[len] = *format;
        output[len + 1] = '\0';
        format++;
    }
    
    printf("%s\n", output);
}

char* createFormattedString(const char *format, char **vars, int varCount) {
    char *output = malloc(1024);
    output[0] = '\0';
    
    while (*format) {
        if (*format == '%' && strncmp(format, "%var", 4) == 0) {
            format += 4;
            int varNum = 0;
            while (*format >= '0' && *format <= '9') {
                varNum = varNum * 10 + (*format - '0');
                format++;
            }
            
            if (varNum < 1 || varNum > varCount) {
                free(output);
                return NULL;
            }
            
            Variable *var = findVariable(vars[varNum - 1]);
            if (var) {
                if (var->type == INT) {
                    char numStr[32];
                    sprintf(numStr, "%d", var->value.intValue);
                    strcat(output, numStr);
                } else if (var->type == FLOAT) {
                    char numStr[32];
                    sprintf(numStr, "%.6f", var->value.floatValue);
                    strcat(output, numStr);
                } else if (var->type == BOOLEAN) {
                    strcat(output, var->value.boolValue ? "true" : "false");
                } else if (var->type == STRING) {
                    strcat(output, var->value.stringValue);
                }
            } else {
                free(output);
                return NULL;
            }
            continue;
        }
        
        int len = strlen(output);
        output[len] = *format;
        output[len + 1] = '\0';
        format++;
    }
    
    return output;
}