#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>  // Add this for isdigit()
#include "lexer_display.h"
#include "lexer_interpret.h"

// Remove duplicate type definitions since they're in lexar_interpret.h
Variable *variables = NULL;
size_t variableCount = 0;

Variable *findVariable(const char *name) {
    for (size_t i = 0; i < variableCount; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return NULL;
}

void addVariable(const char *name, VarType type, void *value) {
    variables = realloc(variables, (variableCount + 1) * sizeof(Variable));
    variables[variableCount].name = strdup(name);
    variables[variableCount].type = type;
    if (type == INT) {
        variables[variableCount].value.intValue = *(int *)value;
    } else if (type == FLOAT) {
        variables[variableCount].value.floatValue = *(float *)value;
    } else if (type == BOOLEAN) {
        variables[variableCount].value.boolValue = *(int *)value;
    } else {
        variables[variableCount].value.stringValue = strdup((char *)value);
    }
    variableCount++;
}

// Add helper functions for float parsing
int isFloat(const char *str) {
    int dots = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            dots++;
        } else if (!isdigit(str[i]) && (i == 0 && str[i] != '-')) {
            return 0;
        }
    }
    return dots == 1;
}

float parseFloat(const char *str) {
    return atof(str);
}

// Function to interpret and execute commands
void interpretCommand(const char *command, int lineNumber) {
    if (strncmp(command, "display(", 8) == 0) {
        const char *closingParenthesis = strchr(command + 8, ')');
        if (closingParenthesis) {
            char *content = (char *)malloc(closingParenthesis - (command + 8) + 1);
            strncpy(content, command + 8, closingParenthesis - (command + 8));
            content[closingParenthesis - (command + 8)] = '\0';

            // First check if it's a formatted string (starts with quote and contains %var)
            if (content[0] == '"' && strstr(content, "%var") != NULL && strchr(content, ',') != NULL) {
                char *format = strtok(content, ",");
                format[strlen(format)-1] = '\0';
                format++;
                
                // Parse variables
                char *vars[10];  // Maximum 10 variables
                int varCount = 0;
                char *var;
                while ((var = strtok(NULL, ",")) != NULL) {
                    while (*var == ' ') var++;  // Skip leading spaces
                    vars[varCount++] = var;
                }
                
                displayFormatted(format, vars, varCount);
            } else {
                // Handle regular display cases
                char *endptr;
                long intValue = strtol(content, &endptr, 10);
                if (*endptr == '\0') {
                    displayInt((int)intValue);
                } else if (isFloat(content)) {
                    displayFloat(parseFloat(content));
                } else if ((content[0] == '"' && content[strlen(content)-1] == '"') || 
                          (content[0] == '\'' && content[strlen(content)-1] == '\'')) {
                    content[strlen(content)-1] = '\0';
                    display(content + 1);
                } else {
                    printf("Syntax error on line %d: invalid format\n", lineNumber);
                    exit(EXIT_FAILURE);
                }
            }
            free(content);
        } else {
            printf("Syntax error on line %d: missing closing parenthesis\n", lineNumber);
            exit(EXIT_FAILURE);
        }
    } else if (strchr(command, '=') != NULL) {
        char *equalsSign = strchr(command, '=');
        size_t nameLength = equalsSign - command;
        char *name = (char *)malloc(nameLength + 1);
        strncpy(name, command, nameLength);
        name[nameLength] = '\0';

        // Trim trailing whitespace from variable name
        char *end = name + nameLength - 1;
        while (end > name && (*end == ' ' || *end == '\t')) {
            *end = '\0';
            end--;
        }

        // Trim leading whitespace from variable name
        char *start = name;
        while (*start == ' ' || *start == '\t') start++;
        if (start != name) {
            memmove(name, start, strlen(start) + 1);
        }

        char *value = equalsSign + 1;
        while (*value == ' ') value++; // Skip leading spaces

        // Add boolean value check before other checks
        if (strcmp(value, "true") == 0) {
            int boolValue = 1;
            addVariable(name, BOOLEAN, &boolValue);
        } else if (strcmp(value, "false") == 0) {
            int boolValue = 0;
            addVariable(name, BOOLEAN, &boolValue);
        } else if (value[0] == '(' && strchr(value, ',') != NULL) {
            char *content = strdup(value + 1);
            char *closingParen = strrchr(content, ')');
            if (closingParen) {
                *closingParen = '\0';
                
                // Parse format and variables
                char *format = strtok(content, ",");
                if (format[0] == '"' && format[strlen(format)-1] == '"') {
                    format[strlen(format)-1] = '\0';
                    format++;
                    
                    char *vars[10];
                    int varCount = 0;
                    char *var;
                    while ((var = strtok(NULL, ",")) != NULL) {
                        while (*var == ' ') var++;
                        vars[varCount++] = var;
                    }
                    
                    char *result = createFormattedString(format, vars, varCount);
                    if (result) {
                        addVariable(name, STRING, result);
                        free(result);
                    } else {
                        printf("Error: Failed to create formatted string on line %d\n", lineNumber);
                        exit(EXIT_FAILURE);
                    }
                }
                free(content);
            }
        } else {
            // Check if value is an integer
            char *endptr;
            long intValue = strtol(value, &endptr, 10);
            if (*endptr == '\0') {
                addVariable(name, INT, &intValue);
            } else if (isFloat(value)) {
                float floatValue = parseFloat(value);
                addVariable(name, FLOAT, &floatValue);
            } else {
                // Check for surrounding quotes
                size_t valueLength = strlen(value);
                if ((value[0] == '"' && value[valueLength - 1] == '"') || 
                    (value[0] == '\'' && value[valueLength - 1] == '\'')) {
                    value[valueLength - 1] = '\0';
                    addVariable(name, STRING, value + 1);
                } else {
                    printf("Syntax error on line %d: missing enclosing quotes or invalid integer in line: %s\n", lineNumber, command);
                    free(name);
                    exit(EXIT_FAILURE);
                }
            }
        }

        free(name);
    } else {
        printf("Unknown command on line %d: %s\n", lineNumber, command);
        exit(EXIT_FAILURE);
    }
}