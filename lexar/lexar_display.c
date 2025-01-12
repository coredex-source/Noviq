#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexar_display.h"
#include "lexar_interpret.h"

// Function to display text
void display(const char *text) {
    printf("%s\n", text);
}

// Function to display integer
void displayInt(int value) {
    printf("%d\n", value);
}

void displayFormatted(const char *format, char **vars, int varCount) {
    char output[1024] = "";
    
    while (*format) {
        if (*format == '%' && strncmp(format, "%var", 4) == 0) {
            format += 4;
            // Get the variable number
            int varNum = 0;
            while (*format >= '0' && *format <= '9') {
                varNum = varNum * 10 + (*format - '0');
                format++;
            }
            
            if (varNum < 1 || varNum > varCount) {
                printf("Error: Invalid variable number %d\n", varNum);
                return;
            }
            
            Variable *var = findVariable(vars[varNum - 1]);
            if (var) {
                if (var->type == INT) {
                    char numStr[32];
                    sprintf(numStr, "%d", var->value.intValue);
                    strcat(output, numStr);
                } else {
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
                } else {
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