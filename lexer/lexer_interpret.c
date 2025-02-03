#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>  // Add this for pow() function
#include "lexer_display.h"
#include "lexer_interpret.h"

// Remove duplicate type definitions since they're in lexar_interpret.h
Variable *variables = NULL;
size_t variableCount = 0;

// Add global line number definition
int currentLineNumber = 0;

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
    // First check if variable already exists
    for (size_t i = 0; i < variableCount; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            // Check if the variable is a constant
            if (variables[i].isConstant) {
                fprintf(stderr, "Error on line %d: Cannot modify constant '%s'\n", 
                        currentLineNumber, name);
                exit(EXIT_FAILURE);
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
            }
            return;
        }
    }

    // If variable doesn't exist, add new one
    variables = realloc(variables, (variableCount + 1) * sizeof(Variable));
    variables[variableCount].name = strdup(name);
    variables[variableCount].type = type;
    variables[variableCount].isConstant = isConstant;  // Set constant flag
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
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

int isLogicalOperator(const char *str) {
    return (strcmp(str, "AND") == 0 || 
            strcmp(str, "OR") == 0 || 
            strcmp(str, "NOT") == 0 ||
            strcmp(str, "&&") == 0 ||
            strcmp(str, "||") == 0 ||
            strcmp(str, "!") == 0);
}

int isComparisonOperator(const char *str) {
    return (strcmp(str, ">") == 0 || 
            strcmp(str, "<") == 0 || 
            strcmp(str, ">=") == 0 || 
            strcmp(str, "<=") == 0 || 
            strcmp(str, "==") == 0);
}

Variable performOperation(Variable *left, Variable *right, const char *operator) {
    Variable result;
    
    // Type checking
    if (left->type == STRING || right->type == STRING) {
        fprintf(stderr, "Error on line %d: Cannot perform arithmetic operations with strings\n", currentLineNumber);
        exit(EXIT_FAILURE);  // Changed from exit(1)
    }

    if (left->type == BOOLEAN || right->type == BOOLEAN) {
        fprintf(stderr, "Error on line %d: Cannot perform arithmetic operations with booleans\n", currentLineNumber);
        exit(EXIT_FAILURE);  // Changed from exit(1)
    }
    
    float leftVal = (left->type == FLOAT) ? left->value.floatValue : (float)left->value.intValue;
    float rightVal = (right->type == FLOAT) ? right->value.floatValue : (float)right->value.intValue;
    
    result.type = FLOAT;

    if (strcmp(operator, "**") == 0) {
        result.value.floatValue = pow(leftVal, rightVal);
    } else if (strcmp(operator, "//") == 0) {
        if (rightVal == 0) {
            fprintf(stderr, "Error on line %d: Division by zero\n", currentLineNumber);
            exit(EXIT_FAILURE);  // Changed from exit(1)
        }
        result.type = INT;
        result.value.intValue = (int)(leftVal / rightVal);
    } else if (strcmp(operator, "%") == 0) {
        if (rightVal == 0) {
            fprintf(stderr, "Error: Modulo by zero\n");
            exit(EXIT_FAILURE);
        }
        result.type = INT;
        result.value.intValue = (int)leftVal % (int)rightVal;
    } else {
        switch(operator[0]) {
            case '+': result.value.floatValue = leftVal + rightVal; break;
            case '-': result.value.floatValue = leftVal - rightVal; break;
            case '*': result.value.floatValue = leftVal * rightVal; break;
            case '/':
                if (rightVal == 0) {
                    fprintf(stderr, "Error: Division by zero\n");
                    exit(EXIT_FAILURE);
                }
                result.value.floatValue = leftVal / rightVal;
                break;
        }
    }

    // Convert to INT if result is a whole number and operation isn't division
    if (result.type == FLOAT && operator[0] != '/' && 
        result.value.floatValue == (int)result.value.floatValue) {
        result.type = INT;
        result.value.intValue = (int)result.value.floatValue;
    }
    
    return result;
}

Variable performLogicalOperation(Variable *left, Variable *right, const char *operator) {
    Variable result;
    result.type = BOOLEAN;

    // Convert operands to boolean values
    int leftBool = 0, rightBool = 0;

    if (left) {
        switch(left->type) {
            case BOOLEAN: leftBool = left->value.boolValue; break;
            case INT: leftBool = left->value.intValue != 0; break;
            case FLOAT: leftBool = left->value.floatValue != 0; break;
            case STRING: leftBool = strlen(left->value.stringValue) > 0; break;
        }
    }

    if (right) {
        switch(right->type) {
            case BOOLEAN: rightBool = right->value.boolValue; break;
            case INT: rightBool = right->value.intValue != 0; break;
            case FLOAT: rightBool = right->value.floatValue != 0; break;
            case STRING: rightBool = strlen(right->value.stringValue) > 0; break;
        }
    }

    if (strcmp(operator, "AND") == 0) {
        result.value.boolValue = leftBool && rightBool;
    } else if (strcmp(operator, "OR") == 0) {
        result.value.boolValue = leftBool || rightBool;
    } else if (strcmp(operator, "NOT") == 0) {
        result.value.boolValue = !leftBool;
    }

    return result;
}

Variable performComparison(Variable *left, Variable *right, const char *operator) {
    Variable result;
    result.type = BOOLEAN;

    // Convert operands to comparable values
    float leftVal, rightVal;

    switch(left->type) {
        case INT: leftVal = (float)left->value.intValue; break;
        case FLOAT: leftVal = left->value.floatValue; break;
        case BOOLEAN: leftVal = (float)left->value.boolValue; break;
        default:
            fprintf(stderr, "Error on line %d: Cannot compare string values\n", currentLineNumber);
            exit(EXIT_FAILURE);
    }

    switch(right->type) {
        case INT: rightVal = (float)right->value.intValue; break;
        case FLOAT: rightVal = right->value.floatValue; break;
        case BOOLEAN: rightVal = (float)right->value.boolValue; break;
        default:
            fprintf(stderr, "Error on line %d: Cannot compare string values\n", currentLineNumber);
            exit(EXIT_FAILURE);
    }

    if (strcmp(operator, ">") == 0) {
        result.value.boolValue = leftVal > rightVal;
    } else if (strcmp(operator, "<") == 0) {
        result.value.boolValue = leftVal < rightVal;
    } else if (strcmp(operator, ">=") == 0) {
        result.value.boolValue = leftVal >= rightVal;
    } else if (strcmp(operator, "<=") == 0) {
        result.value.boolValue = leftVal <= rightVal;
    } else if (strcmp(operator, "==") == 0) {
        result.value.boolValue = leftVal == rightVal;
    }

    return result;
}

Variable *evaluateExpression(const char *expr) {
    char *trimmed = strdup(expr);
    char *ptr = trimmed;
    
    while (*ptr == ' ' || *ptr == '\t') ptr++;

    // Check if it's just a negative number
    if (ptr[0] == '-' && isdigit(ptr[1]) && !strchr(ptr + 1, '+') && 
        !strchr(ptr + 1, '-') && !strchr(ptr + 1, '*') && !strchr(ptr + 1, '/')) {
        Variable *result = malloc(sizeof(Variable));
        if (isFloat(ptr)) {
            result->type = FLOAT;
            result->value.floatValue = parseFloat(ptr);
        } else {
            result->type = INT;
            result->value.intValue = atoi(ptr);
        }
        free(trimmed);
        return result;
    }

    // First check for comparison operators
    char *gtOp = strstr(ptr, ">=");
    char *ltOp = strstr(ptr, "<=");
    char *eqOp = strstr(ptr, "==");
    char *gt = strstr(ptr, ">");
    char *lt = strstr(ptr, "<");
    
    char *op = NULL;
    const char *opStr = NULL;
    int opLen = 1;

    // Find the leftmost operator
    if (gtOp && (!op || gtOp < op)) { op = gtOp; opStr = ">="; opLen = 2; }
    if (ltOp && (!op || ltOp < op)) { op = ltOp; opStr = "<="; opLen = 2; }
    if (eqOp && (!op || eqOp < op)) { op = eqOp; opStr = "=="; opLen = 2; }
    if (gt && (!op || gt < op) && (gt != gtOp)) { op = gt; opStr = ">"; opLen = 1; }
    if (lt && (!op || lt < op) && (lt != ltOp)) { op = lt; opStr = "<"; opLen = 1; }

    if (op) {
        *op = '\0';
        char *leftStr = ptr;
        char *rightStr = op + opLen;

        Variable *left = evaluateExpression(leftStr);
        Variable *right = evaluateExpression(rightStr);

        if (left && right) {
            Variable *result = malloc(sizeof(Variable));
            *result = performComparison(left, right, opStr);
            free(left);
            free(right);
            free(trimmed);
            return result;
        }
        if (left) free(left);
        if (right) free(right);
        free(trimmed);
        return NULL;
    }

    // Check for NOT/! operator first
    if (strncmp(ptr, "NOT ", 4) == 0 || strncmp(ptr, "! ", 2) == 0 || ptr[0] == '!') {
        // Skip the minus if it's followed by a number (negative number)
        if (ptr[0] == '-' && isdigit(ptr[1])) {
            // Do nothing, let it fall through to number parsing
        } else {
            Variable *operand = evaluateExpression(ptr + (ptr[0] == 'N' ? 4 : (ptr[0] == '!' && ptr[1] == ' ' ? 2 : 1)));
            if (operand) {
                Variable *result = malloc(sizeof(Variable));
                *result = performLogicalOperation(operand, NULL, "NOT");
                free(operand);
                free(trimmed);
                return result;
            }
        }
    }

    // Check for AND/OR/&&/|| operators
    char *andOp = strstr(ptr, " AND ");
    char *orOp = strstr(ptr, " OR ");
    char *andSymbolOp = strstr(ptr, "&&");
    char *orSymbolOp = strstr(ptr, "||");
    op = NULL;
    opStr = NULL;
    opLen = 0;

    // Find the leftmost operator
    if (andOp && (!op || andOp < op)) { op = andOp; opStr = "AND"; opLen = 5; }
    if (orOp && (!op || orOp < op)) { op = orOp; opStr = "OR"; opLen = 4; }
    if (andSymbolOp && (!op || andSymbolOp < op)) { op = andSymbolOp; opStr = "AND"; opLen = 2; }
    if (orSymbolOp && (!op || orSymbolOp < op)) { op = orSymbolOp; opStr = "OR"; opLen = 2; }

    if (op) {
        *op = '\0';
        char *leftStr = ptr;
        char *rightStr = op + opLen;

        // Trim spaces around operators
        while (*rightStr == ' ') rightStr++;

        Variable *left = evaluateExpression(leftStr);
        Variable *right = evaluateExpression(rightStr);

        if (left && right) {
            Variable *result = malloc(sizeof(Variable));
            *result = performLogicalOperation(left, right, opStr);
            free(left);
            free(right);
            free(trimmed);
            return result;
        }
        if (left) free(left);
        if (right) free(right);
        free(trimmed);
        return NULL;
    }

    // Check for two-character operators first
    op = NULL;
    for (char *c = ptr; *c; c++) {
        if ((c[0] == '*' && c[1] == '*') || 
            (c[0] == '/' && c[1] == '/')) {
            op = c;
            break;
        }
        // Check for modulo or single operators
        if (*c == '%' || (*c == '*' || *c == '/' || *c == '+' || *c == '-')) {
            if (*c == '-' && (c == ptr || isOperator(*(c-1)))) {
                continue;  // Skip leading minus or minus after operator
            }
            if (!op) op = c;
        }
    }

    if (!op) {
        // First check if it's a simple variable or value
        if (!strchr(ptr, '+') && !strchr(ptr + 1, '-') && !strchr(ptr, '*') && !strchr(ptr, '/')) {
            Variable *var = findVariable(ptr);
            if (var) {
                Variable *result = malloc(sizeof(Variable));
                *result = *var;
                free(trimmed);
                return result;
            }
            // Try parsing as number (including negative numbers)
            if (isdigit(*ptr) || (*ptr == '-' && isdigit(*(ptr + 1)))) {
                Variable *result = malloc(sizeof(Variable));
                if (isFloat(ptr)) {
                    result->type = FLOAT;
                    result->value.floatValue = parseFloat(ptr);
                } else {
                    result->type = INT;
                    result->value.intValue = atoi(ptr);
                }
                free(trimmed);
                return result;
            }
            free(trimmed);
            return NULL;
        }
    }

    // Handle two-character operators
    char operator[3] = {0};
    char *rightStr;
    if ((op[0] == '*' && op[1] == '*') || (op[0] == '/' && op[1] == '/')) {
        operator[0] = op[0];
        operator[1] = op[1];
        *op = '\0';
        rightStr = op + 2;
    } else {
        operator[0] = *op;
        *op = '\0';
        rightStr = op + 1;
    }
    char *leftStr = ptr;

    // Split the expression and save the operator
    char *left_trimmed = strdup(leftStr);
    char *right_trimmed = strdup(rightStr);
    
    char *left_ptr = left_trimmed;
    char *right_ptr = right_trimmed;
    
    // Trim left operand
    while (*left_ptr == ' ' || *left_ptr == '\t') left_ptr++;
    char *left_end = left_ptr + strlen(left_ptr) - 1;
    while (left_end > left_ptr && (*left_end == ' ' || *left_end == '\t')) {
        *left_end = '\0';
        left_end--;
    }
    
    // Trim right operand
    while (*right_ptr == ' ' || *right_ptr == '\t') right_ptr++;
    char *right_end = right_ptr + strlen(right_ptr) - 1;
    while (right_end > right_ptr && (*right_end == ' ' || *right_end == '\t')) {
        *right_end = '\0';
        right_end--;
    }

    // Get operands
    Variable *left = findVariable(left_ptr);
    Variable *right = findVariable(right_ptr);
    Variable temp_left, temp_right;

    // Handle numeric literals for left operand
    if (!left && (isdigit(*left_ptr) || (*left_ptr == '-' && isdigit(*(left_ptr + 1))))) {
        temp_left.type = isFloat(left_ptr) ? FLOAT : INT;
        if (temp_left.type == FLOAT)
            temp_left.value.floatValue = parseFloat(left_ptr);
        else
            temp_left.value.intValue = atoi(left_ptr);
        left = &temp_left;
    }

    // Handle numeric literals for right operand
    if (!right && (isdigit(*right_ptr) || (*right_ptr == '-' && isdigit(*(right_ptr + 1))))) {
        temp_right.type = isFloat(right_ptr) ? FLOAT : INT;
        if (temp_right.type == FLOAT)
            temp_right.value.floatValue = parseFloat(right_ptr);
        else
            temp_right.value.intValue = atoi(right_ptr);
        right = &temp_right;
    }

    // Clean up
    free(left_trimmed);
    free(right_trimmed);
    
    if (!left || !right) {
        free(trimmed);
        return NULL;
    }

    // Create operator string and perform operation
    Variable *result = malloc(sizeof(Variable));
    *result = performOperation(left, right, operator);

    free(trimmed);
    return result;
}

// Function to interpret and execute commands
void processInput(const char *prompt, const char *varName) {
    char buffer[1024];
    
    // Print the prompt
    printf("%s", prompt);
    fflush(stdout);
    
    // Get input
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        // Try to parse as number first
        char *endptr;
        long intValue = strtol(buffer, &endptr, 10);
        
        if (*endptr == '\0') {
            // It's an integer
            int value = (int)intValue;
            addVariable(varName, INT, &value, 0);
        } else if (isFloat(buffer)) {
            // It's a float
            float value = parseFloat(buffer);
            addVariable(varName, FLOAT, &value, 0);
        } else if (strcmp(buffer, "true") == 0) {
            // It's boolean true
            int value = 1;
            addVariable(varName, BOOLEAN, &value, 0);
        } else if (strcmp(buffer, "false") == 0) {
            // It's boolean false
            int value = 0;
            addVariable(varName, BOOLEAN, &value, 0);
        } else {
            // Treat as string by default
            addVariable(varName, STRING, buffer, 0);
        }
    }
}

void handleInputCommand(const char *args) {
    char *argsTemp = strdup(args);
    char **prompts = NULL;
    char **variables = NULL;
    int count = 0;
    int maxVars = 10;  // Maximum variables to handle
    
    prompts = malloc(maxVars * sizeof(char*));
    variables = malloc(maxVars * sizeof(char*));
    
    char *token = strtok(argsTemp, ",");
    while (token != NULL && count < maxVars) {
        // Skip leading whitespace
        while (*token == ' ') token++;
        
        if (token[0] == '"' || token[0] == '\'') {
            // This is a prompt
            size_t len = strlen(token);
            token[len-1] = '\0';  // Remove closing quote
            prompts[count] = strdup(token + 1);
        } else {
            // This is a variable
            variables[count] = strdup(token);
            count++;
        }
        token = strtok(NULL, ",");
    }
    
    // Process all variables
    for (int i = 0; i < count; i++) {
        // If we have a prompt for this variable, use it
        // Otherwise use a modified version of the first prompt
        if (prompts[i]) {
            processInput(prompts[i], variables[i]);
        } else if (i > 0 && prompts[0]) {
            // For subsequent variables using the same prompt, just show ": "
            processInput(": ", variables[i]);
        } else {
            processInput(prompts[0], variables[i]);
        }
    }
    
    // Cleanup
    for (int i = 0; i < count; i++) {
        if (prompts[i]) free(prompts[i]);
        if (variables[i]) free(variables[i]);
    }
    free(prompts);
    free(variables);
    free(argsTemp);
}

int evaluateCondition(const char *condition) {
    // First try to find it as a boolean variable
    Variable *var = findVariable(condition);
    if (var) {
        if (var->type == BOOLEAN) return var->value.boolValue;
        if (var->type == INT) return var->value.intValue != 0;
        if (var->type == FLOAT) return var->value.floatValue != 0;
        if (var->type == STRING) return strlen(var->value.stringValue) > 0;
    }

    // Try to evaluate as an expression
    Variable *result = evaluateExpression(condition);
    if (result) {
        int value;
        if (result->type == BOOLEAN) value = result->value.boolValue;
        else if (result->type == INT) value = result->value.intValue != 0;
        else if (result->type == FLOAT) value = result->value.floatValue != 0;
        else value = 0;
        free(result);
        return value;
    }

    return 0;
}

void executeIfBlock(const char *condition, const char *block) {
    // Remove any leading/trailing spaces from condition
    while (*condition == ' ' || *condition == '\t') condition++;
    char *conditionEnd = (char *)condition + strlen(condition) - 1;
    while (conditionEnd > condition && (*conditionEnd == ' ' || *conditionEnd == '\t')) conditionEnd--;
    
    // Create temporary condition string
    char *tempCondition = (char *)malloc(conditionEnd - condition + 2);
    strncpy(tempCondition, condition, conditionEnd - condition + 1);
    tempCondition[conditionEnd - condition + 1] = '\0';

    if (evaluateCondition(tempCondition)) {
        // Extract and execute if block
        char *ifContent = strstr(block, "{");
        if (ifContent) {
            // Find the matching closing brace by counting nested braces
            char *endIf = ifContent + 1;
            int braceCount = 1;
            int inString = 0;
            char stringChar = 0;

            while (*endIf && braceCount > 0) {
                // Handle string literals
                if ((*endIf == '"' || *endIf == '\'') && !inString) {
                    inString = 1;
                    stringChar = *endIf;
                } else if (inString && *endIf == stringChar) {
                    inString = 0;
                }
                
                // Only count braces outside of strings
                if (!inString) {
                    if (*endIf == '{') braceCount++;
                    if (*endIf == '}') braceCount--;
                }
                
                // Move to next character if we haven't found the matching brace
                if (braceCount > 0) endIf++;
            }
            
            // Execute if block content
            char *blockContent = malloc(endIf - ifContent);
            strncpy(blockContent, ifContent + 1, endIf - ifContent - 2);
            blockContent[endIf - ifContent - 2] = '\0';
            
            char *line = strtok(blockContent, "\n");
            while (line) {
                while (*line == ' ' || *line == '\t') line++;
                if (*line) {
                    interpretCommand(line, currentLineNumber);
                }
                line = strtok(NULL, "\n");
            }
            free(blockContent);
        }
    } else {
        // Find and execute else block if it exists
        char *elseContent = strstr(block, "else");
        // Skip any nested else blocks by ensuring we're at the right nesting level
        while (elseContent) {
            // Count braces between start and else to verify it's the correct else
            const char *temp = block;  // Changed to const char*
            int braceCount = 0;
            while (temp < elseContent) {
                if (*temp == '{') braceCount++;
                if (*temp == '}') braceCount--;
                temp++;
            }
            // If braceCount is 0, we found our else
            if (braceCount == 0) break;
            elseContent = strstr(elseContent + 4, "else");
        }

        if (elseContent) {
            char *elseBlock = strchr(elseContent, '{');
            if (elseBlock) {
                // Find the end of the else block using brace counting
                char *endElse = elseBlock + 1;
                int braceCount = 1;
                while (*endElse && braceCount > 0) {
                    if (*endElse == '{') braceCount++;
                    if (*endElse == '}') braceCount--;
                    endElse++;
                }

                // Execute else block content
                char *blockContent = malloc(endElse - elseBlock);
                strncpy(blockContent, elseBlock + 1, endElse - elseBlock - 2);
                blockContent[endElse - elseBlock - 2] = '\0';
                
                char *line = strtok(blockContent, "\n");
                while (line) {
                    while (*line == ' ' || *line == '\t') line++;
                    if (*line) {
                        interpretCommand(line, currentLineNumber);
                    }
                    line = strtok(NULL, "\n");
                }
                free(blockContent);
            }
        }
    }
    
    free(tempCondition);
}

void interpretCommand(const char *command, int lineNumber) {
    currentLineNumber = lineNumber;
    
    if (strncmp(command, "if", 2) == 0) {
        char *openParen = strchr(command, '(');
        if (!openParen) {
            fprintf(stderr, "Error on line %d: Missing opening parenthesis\n", lineNumber);
            exit(EXIT_FAILURE);
        }

        char *closeParen = strchr(openParen, ')');
        if (!closeParen) {
            fprintf(stderr, "Error on line %d: Missing closing parenthesis\n", lineNumber);
            exit(EXIT_FAILURE);
        }

        char *openBrace = strchr(closeParen, '{');
        if (!openBrace) {
            fprintf(stderr, "Error on line %d: Missing opening brace\n", lineNumber);
            exit(EXIT_FAILURE);
        }

        char *closeBrace = strrchr(command, '}');  // Use strrchr to find last }
        if (!closeBrace) {
            fprintf(stderr, "Error on line %d: Missing closing brace\n", lineNumber);
            exit(EXIT_FAILURE);
        }

        // Extract condition
        size_t conditionLen = closeParen - (openParen + 1);
        char *condition = (char *)malloc(conditionLen + 1);
        strncpy(condition, openParen + 1, conditionLen);
        condition[conditionLen] = '\0';

        // Extract block content
        char *block = malloc(strlen(openBrace));
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
            printf("Syntax error on line %d: constant declaration requires initialization\n", lineNumber);
            exit(EXIT_FAILURE);
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
                addVariable(name, FLOAT, &floatVal, 1);  // 1 for constant
            } else {
                int intVal = result->value.intValue;
                addVariable(name, INT, &intVal, 1);  // 1 for constant
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
            printf("Syntax error on line %d: missing closing parenthesis\n", lineNumber);
            exit(EXIT_FAILURE);
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

        // Check for arithmetic expression first
        Variable *result = evaluateExpression(value);
        if (result) {
            if (result->type == FLOAT) {
                float floatVal = result->value.floatValue;
                addVariable(name, FLOAT, &floatVal, 0);
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
                size_t valueLength = strlen(value);
                value[valueLength - 1] = '\0';
                addVariable(name, STRING, value + 1, 0);
            }
        }

        free(name);
    } else {
        printf("Unknown command on line %d: %s\n", lineNumber, command);
        exit(EXIT_FAILURE);
    }
}