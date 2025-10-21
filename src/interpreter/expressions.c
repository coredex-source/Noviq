#include "expressions.h"
#include "variables.h"
#include "arithmetic.h"
#include "logical.h"
#include "comparison.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

int isFloat(const char *str) {
    int dots = 0;
    int i = 0;
    
    // Handle negative numbers
    if (str[0] == '-') i = 1;
    
    for (; str[i] != '\0'; i++) {
        if (str[i] == '.') {
            dots++;
        } else if (!isdigit(str[i])) {
            return 0;
        }
    }
    return dots == 1;
}

float parseFloat(const char *str) {
    return atof(str);
}

int isExpression(const char *str) {
    // Skip leading whitespace
    while (*str == ' ' || *str == '\t') str++;
    
    // Check if it's just a negative number
    if (*str == '-' && isdigit(*(str + 1))) {
        str++;  // Skip the minus
        while (isdigit(*str) || *str == '.') str++;  // Skip the number
        // If we've reached the end, it's just a negative number
        return *str != '\0';
    }

    for (int i = 0; str[i]; i++) {
        // Check for two-character operators
        if (str[i] == '*' && str[i+1] == '*') return 1;  // **
        if (str[i] == '/' && str[i+1] == '/') return 1;  // //
        // Check for single character operators except leading minus
        if (i > 0 && strchr("+-*/%", str[i])) return 1;
        // Check for operator at start only if not a negative number
        if (i == 0 && strchr("+*/%", str[i])) return 1;
    }
    return 0;
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
        result->name = NULL;
        result->isConstant = 0;
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
                // Deep copy string if needed
                if (var->type == STRING) {
                    result->value.stringValue = strdup(var->value.stringValue);
                }
                result->name = NULL;
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
                result->name = NULL;
                result->isConstant = 0;
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
        temp_left.name = NULL;
        temp_left.isConstant = 0;
        left = &temp_left;
    }

    // Handle numeric literals for right operand
    if (!right && (isdigit(*right_ptr) || (*right_ptr == '-' && isdigit(*(right_ptr + 1))))) {
        temp_right.type = isFloat(right_ptr) ? FLOAT : INT;
        if (temp_right.type == FLOAT)
            temp_right.value.floatValue = parseFloat(right_ptr);
        else
            temp_right.value.intValue = atoi(right_ptr);
        temp_right.name = NULL;
        temp_right.isConstant = 0;
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
    *result = performArithmeticOperation(left, right, operator);

    free(trimmed);
    return result;
}
