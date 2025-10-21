#include "display.h"
#include "variables.h"
#include "expressions.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void display(const char *text) {
    printf("%s\n", text);
}

void displayInt(int value) {
    printf("%d\n", value);
}

void displayFloat(float value) {
    printf("%f\n", value);
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
                reportError(ERROR_RUNTIME, getCurrentLine(), 
                           "Invalid variable number %d (expected 1-%d)", varNum, varCount);
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
                    reportError(ERROR_RUNTIME, getCurrentLine(), 
                               "Invalid expression '%s'", expr);
                }
            } else {
                // Handle simple variable
                Variable *var = findVariable(expr);
                if (!var) {
                    reportError(ERROR_UNDEFINED_VAR, getCurrentLine(), 
                               "Variable '%s' not found", expr);
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
