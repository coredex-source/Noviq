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

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

Variable performOperation(Variable *left, Variable *right, char operator) {
    Variable result;
    
    // Type checking
    if (left->type == STRING || right->type == STRING) {
        fprintf(stderr, "Error: Cannot perform arithmetic operations with strings\n");
        exit(1);
    }

    if (left->type == BOOLEAN || right->type == BOOLEAN) {
        fprintf(stderr, "Error: Cannot perform arithmetic operations with booleans\n");
        exit(1);
    }
    
    // If either operand is float, result is float
    if (left->type == FLOAT || right->type == FLOAT) {
        float leftVal = (left->type == FLOAT) ? left->value.floatValue : 
                                               (float)left->value.intValue;
        float rightVal = (right->type == FLOAT) ? right->value.floatValue : 
                                                 (float)right->value.intValue;
        
        switch(operator) {
            case '+': result.value.floatValue = leftVal + rightVal; break;
            case '-': result.value.floatValue = leftVal - rightVal; break;
            case '*': result.value.floatValue = leftVal * rightVal; break;
            case '/':
                if (rightVal == 0) {
                    fprintf(stderr, "Error: Division by zero\n");
                    exit(1);
                }
                result.value.floatValue = leftVal / rightVal;
                break;
        }
    } else {
        // Both operands are integers
        result.type = INT;
        switch(operator) {
            case '+': result.value.intValue = left->value.intValue + right->value.intValue; break;
            case '-': result.value.intValue = left->value.intValue - right->value.intValue; break;
            case '*': result.value.intValue = left->value.intValue * right->value.intValue; break;
            case '/':
                if (right->value.intValue == 0) {
                    fprintf(stderr, "Error: Division by zero\n");
                    exit(1);
                }
                result.type = FLOAT;
                result.value.floatValue = (float)left->value.intValue / right->value.intValue;
                break;
        }
    }
    return result;
}

Variable *evaluateExpression(const char *expr) {
    char *trimmed = strdup(expr);
    char *ptr = trimmed;
    
    // Skip leading whitespace
    while (*ptr == ' ' || *ptr == '\t') ptr++;
    
    // Find operator while handling whitespace
    char *op = NULL;
    char *c = ptr;
    while (*c) {
        if (isOperator(*c)) {
            // Verify this is not a negative number at start
            if (*c == '-' && c == ptr) {
                c++;
                continue;
            }
            op = c;
            break;
        }
        c++;
    }
    
    if (!op) {
        free(trimmed);
        return NULL;
    }
    
    // Split expression into left and right parts
    char operator = *op;
    *op = '\0';
    char *leftStr = ptr;
    char *rightStr = op + 1;
    
    // Trim trailing whitespace from left operand
    char *leftEnd = op - 1;
    while (leftEnd > leftStr && (*leftEnd == ' ' || *leftEnd == '\t')) {
        *leftEnd = '\0';
        leftEnd--;
    }
    
    // Trim leading whitespace from right operand
    while (*rightStr == ' ' || *rightStr == '\t') rightStr++;
    
    // Trim trailing whitespace from right operand
    char *rightEnd = rightStr + strlen(rightStr) - 1;
    while (rightEnd > rightStr && (*rightEnd == ' ' || *rightEnd == '\t')) {
        *rightEnd = '\0';
        rightEnd--;
    }

    // Create temporary variables for operands
    Variable *left = NULL;
    Variable *right = NULL;
    Variable temp_left, temp_right;
    
    // Parse left operand
    if (isdigit(*leftStr) || (*leftStr == '-' && strlen(leftStr) > 1)) {
        temp_left.type = isFloat(leftStr) ? FLOAT : INT;
        if (temp_left.type == FLOAT)
            temp_left.value.floatValue = parseFloat(leftStr);
        else
            temp_left.value.intValue = atoi(leftStr);
        left = &temp_left;
    } else {
        left = findVariable(leftStr);
    }
    
    // Parse right operand
    if (isdigit(*rightStr) || (*rightStr == '-' && strlen(rightStr) > 1)) {
        temp_right.type = isFloat(rightStr) ? FLOAT : INT;
        if (temp_right.type == FLOAT)
            temp_right.value.floatValue = parseFloat(rightStr);
        else
            temp_right.value.intValue = atoi(rightStr);
        right = &temp_right;
    } else {
        right = findVariable(rightStr);
    }
    
    if (!left || !right) {
        free(trimmed);
        return NULL;
    }
    
    // Perform operation
    Variable *result = malloc(sizeof(Variable));
    *result = performOperation(left, right, operator);
    
    free(trimmed);
    return result;
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
            // Check for arithmetic operations
            Variable *result = evaluateExpression(value);
            if (result) {
                addVariable(name, result->type, &result->value);
                free(result);
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
        }

        free(name);
    } else {
        printf("Unknown command on line %d: %s\n", lineNumber, command);
        exit(EXIT_FAILURE);
    }
}