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
    return strchr(str, '+') || strchr(str, '-') || 
           strchr(str, '*') || strchr(str, '/');
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
                printf("Error: Invalid variable number %d\n", varNum);
                return;
            }

            // Trim whitespace from the variable/expression
            char *trimmed = strdup(vars[varNum - 1]);
            char *start = trimmed;
            while (*start == ' ') start++;
            char *end = start + strlen(start) - 1;
            while (end > start && *end == ' ') end--;
            *(end + 1) = '\0';

            // Check if the argument is an expression
            if (isExpression(start)) {
                Variable *result = evaluateExpression(start);
                if (result) {
                    if (result->type == INT) {
                        char numStr[32];
                        sprintf(numStr, "%d", result->value.intValue);
                        strcat(output, numStr);
                    } else if (result->type == FLOAT) {
                        char numStr[32];
                        sprintf(numStr, "%.6f", result->value.floatValue);
                        strcat(output, numStr);
                    }
                    free(result);
                } else {
                    printf("Error: Failed to evaluate expression '%s'\n", start);
                }
                free(trimmed);
                continue;
            }
            free(trimmed);

            // Regular variable lookup
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
                printf("Error: Variable '%s' not found\n", vars[varNum - 1]);
                return;
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