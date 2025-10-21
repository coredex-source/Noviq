#include "variables.h"
#include "error.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static Variable *variables = NULL;
static size_t variableCount = 0;

Variable *findVariable(const char *name) {
    char *trimmed = strdup(name);
    char *ptr = trimmed;
    
    // Trim leading whitespace
    while (*ptr == ' ' || *ptr == '\t') ptr++;
    
    // Trim trailing whitespace
    char *end = ptr + strlen(ptr) - 1;
    while (end > ptr && (*end == ' ' || *end == '\t')) {
        *end = '\0';
        end--;
    }

    for (size_t i = 0; i < variableCount; i++) {
        if (strcmp(variables[i].name, ptr) == 0) {
            free(trimmed);
            return &variables[i];
        }
    }
    free(trimmed);
    return NULL;
}

void addVariable(const char *name, VarType type, void *value, int isConstant) {
    // Check if we've reached max variables
    if (variableCount >= NOVIQ_MAX_VARIABLES) {
        reportError(ERROR_MEMORY, getCurrentLine(), 
                   "Maximum number of variables (%d) exceeded", NOVIQ_MAX_VARIABLES);
    }

    // First check if variable already exists
    for (size_t i = 0; i < variableCount; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            // Check if the variable is a constant
            if (variables[i].isConstant) {
                reportError(ERROR_CONST_MODIFY, getCurrentLine(), 
                           "Cannot modify constant '%s'", name);
            }
            
            // Update value
            if (variables[i].type == STRING) {
                free(variables[i].value.stringValue);
            }
            variables[i].type = type;
            if (type == INT) {
                variables[i].value.intValue = *(int *)value;
            } else if (type == FLOAT) {
                variables[i].value.floatValue = *(float *)value;
            } else if (type == BOOLEAN) {
                variables[i].value.boolValue = *(int *)value;
            } else {
                variables[i].value.stringValue = strdup((char *)value);
                if (!variables[i].value.stringValue) {
                    reportError(ERROR_MEMORY, getCurrentLine(), "Failed to allocate memory for string");
                }
            }
            return;
        }
    }

    // If variable doesn't exist, add new one
    variables = realloc(variables, (variableCount + 1) * sizeof(Variable));
    if (!variables) {
        reportError(ERROR_MEMORY, getCurrentLine(), "Failed to allocate memory for variables");
    }

    variables[variableCount].name = strdup(name);
    if (!variables[variableCount].name) {
        reportError(ERROR_MEMORY, getCurrentLine(), "Failed to allocate memory for variable name");
    }

    variables[variableCount].type = type;
    variables[variableCount].isConstant = isConstant;
    
    if (type == INT) {
        variables[variableCount].value.intValue = *(int *)value;
    } else if (type == FLOAT) {
        variables[variableCount].value.floatValue = *(float *)value;
    } else if (type == BOOLEAN) {
        variables[variableCount].value.boolValue = *(int *)value;
    } else {
        variables[variableCount].value.stringValue = strdup((char *)value);
        if (!variables[variableCount].value.stringValue) {
            reportError(ERROR_MEMORY, getCurrentLine(), "Failed to allocate memory for string value");
        }
    }
    variableCount++;
}

void freeVariable(Variable *var) {
    if (var && var->type == STRING && var->value.stringValue) {
        free(var->value.stringValue);
        var->value.stringValue = NULL;
    }
}

void freeAllVariables(void) {
    for (size_t i = 0; i < variableCount; i++) {
        free(variables[i].name);
        freeVariable(&variables[i]);
    }
    free(variables);
    variables = NULL;
    variableCount = 0;
}

size_t getVariableCount(void) {
    return variableCount;
}
