#include "interpreter.h"
#include "variables.h"
#include "expressions.h"
#include "display.h"
#include "input.h"
#include "control_flow.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void interpretCommand(const char *command) {
    if (strncmp(command, "if", 2) == 0) {
        char *openParen = strchr(command, '(');
        if (!openParen) {
            reportError(ERROR_SYNTAX, getCurrentLine(), "Missing opening parenthesis");
        }

        // Find the matching closing parenthesis for the if condition
        // Need to count nested parentheses, but skip parentheses inside strings
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
            reportError(ERROR_SYNTAX, getCurrentLine(), "Missing closing parenthesis");
        }

        char *openBrace = strchr(closeParen, '{');
        if (!openBrace) {
            reportError(ERROR_SYNTAX, getCurrentLine(), "Missing opening brace");
        }

        char *closeBrace = strrchr(command, '}');
        if (!closeBrace) {
            reportError(ERROR_SYNTAX, getCurrentLine(), "Missing closing brace");
        }

        // Extract condition
        size_t conditionLen = closeParen - (openParen + 1);
        char *condition = (char *)malloc(conditionLen + 1);
        strncpy(condition, openParen + 1, conditionLen);
        condition[conditionLen] = '\0';

        // Extract block content
        char *block = malloc(strlen(openBrace) + 1);
        strcpy(block, openBrace);
        executeIfBlock(condition, block);

        free(condition);
        free(block);
        return;
    }

    if (strncmp(command, "const ", 6) == 0) {
        // Handle constant declaration
        const char *declaration = command + 6;
        char *equalsSign = strchr(declaration, '=');
        if (!equalsSign) {
            reportError(ERROR_SYNTAX, getCurrentLine(), 
                       "Constant declaration requires initialization");
        }

        size_t nameLength = equalsSign - declaration;
        char *name = (char *)malloc(nameLength + 1);
        strncpy(name, declaration, nameLength);
        name[nameLength] = '\0';

        // Trim name
        char *end = name + nameLength - 1;
        while (end > name && (*end == ' ' || *end == '\t')) {
            *end = '\0';
            end--;
        }
        char *start = name;
        while (*start == ' ' || *start == '\t') start++;
        if (start != name) {
            memmove(name, start, strlen(start) + 1);
        }

        char *value = equalsSign + 1;
        while (*value == ' ') value++;

        // Evaluate and add constant
        Variable *result = evaluateExpression(value);
        if (result) {
            if (result->type == FLOAT) {
                float floatVal = result->value.floatValue;
                addVariable(name, FLOAT, &floatVal, 1);
            } else if (result->type == BOOLEAN) {
                int boolVal = result->value.boolValue;
                addVariable(name, BOOLEAN, &boolVal, 1);
            } else {
                int intVal = result->value.intValue;
                addVariable(name, INT, &intVal, 1);
            }
            free(result);
        } else {
            // Handle literal values
            if (strcmp(value, "true") == 0) {
                int boolValue = 1;
                addVariable(name, BOOLEAN, &boolValue, 1);
            } else if (strcmp(value, "false") == 0) {
                int boolValue = 0;
                addVariable(name, BOOLEAN, &boolValue, 1);
            } else if (isdigit(value[0]) || (value[0] == '-' && isdigit(value[1]))) {
                if (isFloat(value)) {
                    float floatValue = parseFloat(value);
                    addVariable(name, FLOAT, &floatValue, 1);
                } else {
                    int intValue = atoi(value);
                    addVariable(name, INT, &intValue, 1);
                }
            } else if (value[0] == '"' || value[0] == '\'') {
                size_t valueLength = strlen(value);
                value[valueLength - 1] = '\0';
                addVariable(name, STRING, value + 1, 1);
            }
        }
        free(name);
    } else if (strncmp(command, "input(", 6) == 0) {
        const char *closingParenthesis = strchr(command + 6, ')');
        if (closingParenthesis) {
            char *content = (char *)malloc(closingParenthesis - (command + 6) + 1);
            strncpy(content, command + 6, closingParenthesis - (command + 6));
            content[closingParenthesis - (command + 6)] = '\0';
            handleInputCommand(content);
            free(content);
        } else {
            reportError(ERROR_SYNTAX, getCurrentLine(), "Missing closing parenthesis");
        }
    } else if (strncmp(command, "display(", 8) == 0) {
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
                    reportError(ERROR_SYNTAX, getCurrentLine(), "Invalid display format");
                }
            }
            free(content);
        } else {
            reportError(ERROR_SYNTAX, getCurrentLine(), "Missing closing parenthesis");
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
        while (*value == ' ') value++;

        // Check for arithmetic expression first
        Variable *result = evaluateExpression(value);
        if (result) {
            if (result->type == FLOAT) {
                float floatVal = result->value.floatValue;
                addVariable(name, FLOAT, &floatVal, 0);
            } else if (result->type == BOOLEAN) {
                int boolVal = result->value.boolValue;
                addVariable(name, BOOLEAN, &boolVal, 0);
            } else {
                int intVal = result->value.intValue;
                addVariable(name, INT, &intVal, 0);
            }
            free(result);
        } else {
            // Handle non-expression assignments
            if (strcmp(value, "true") == 0) {
                int boolValue = 1;
                addVariable(name, BOOLEAN, &boolValue, 0);
            } else if (strcmp(value, "false") == 0) {
                int boolValue = 0;
                addVariable(name, BOOLEAN, &boolValue, 0);
            } else if (isdigit(value[0]) || (value[0] == '-' && isdigit(value[1]))) {
                if (isFloat(value)) {
                    float floatValue = parseFloat(value);
                    addVariable(name, FLOAT, &floatValue, 0);
                } else {
                    int intValue = atoi(value);
                    addVariable(name, INT, &intValue, 0);
                }
            } else if (value[0] == '"' || value[0] == '\'') {
                char quoteChar = value[0];
                char *endQuote = strchr(value + 1, quoteChar);
                if (endQuote) {
                    *endQuote = '\0';
                    addVariable(name, STRING, value + 1, 0);
                } else {
                    // No closing quote found - treat as error or use whole string
                    reportError(ERROR_SYNTAX, getCurrentLine(), "Missing closing quote in string");
                }
            }
        }

        free(name);
    } else {
        reportError(ERROR_SYNTAX, getCurrentLine(), "Unknown command: %s", command);
    }
}
