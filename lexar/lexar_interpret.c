#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexar_display.h"

typedef enum { INT, STRING } VarType;

typedef struct {
    char *name;
    VarType type;
    union {
        int intValue;
        char *stringValue;
    } value;
} Variable;

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
    } else {
        variables[variableCount].value.stringValue = strdup((char *)value);
    }
    variableCount++;
}

// Function to interpret and execute commands
void interpretCommand(const char *command, int lineNumber) {
    if (strncmp(command, "display(", 8) == 0) {
        const char *closingParenthesis = strchr(command + 8, ')');
        if (closingParenthesis) {
            size_t contentLength = closingParenthesis - (command + 8);
            char *content = (char *)malloc(contentLength + 1);
            if (content) {
                strncpy(content, command + 8, contentLength);
                content[contentLength] = '\0';
                
                // Check if content is an integer
                char *endptr;
                long intValue = strtol(content, &endptr, 10);
                if (*endptr == '\0') {
                    displayInt((int)intValue);
                } else {
                    // Check for surrounding quotes
                    if ((content[0] == '"' && content[contentLength - 1] == '"') || 
                        (content[0] == '\'' && content[contentLength - 1] == '\'')) {
                        content[contentLength - 1] = '\0';
                        display(content + 1);
                    } else {
                        printf("Syntax error on line %d: missing enclosing quotes or invalid integer in line: %s\n", lineNumber, command);
                        free(content);
                        exit(EXIT_FAILURE);
                    }
                }
                
                free(content);
            }
        } else {
            printf("Syntax error on line %d: missing closing parenthesis in line: %s\n", lineNumber, command);
            exit(EXIT_FAILURE);
        }
    } else if (strchr(command, '=') != NULL) {
        char *equalsSign = strchr(command, '=');
        size_t nameLength = equalsSign - command;
        char *name = (char *)malloc(nameLength + 1);
        strncpy(name, command, nameLength);
        name[nameLength] = '\0';

        char *value = equalsSign + 1;
        while (*value == ' ') value++; // Skip leading spaces

        // Check if value is an integer
        char *endptr;
        long intValue = strtol(value, &endptr, 10);
        if (*endptr == '\0') {
            addVariable(name, INT, &intValue);
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

        free(name);
    } else {
        printf("Unknown command on line %d: %s\n", lineNumber, command);
        exit(EXIT_FAILURE);
    }
}