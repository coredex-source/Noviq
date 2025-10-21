#include "arithmetic.h"
#include "error.h"
#include <string.h>
#include <math.h>

int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

Variable performArithmeticOperation(Variable *left, Variable *right, const char *operator) {
    Variable result;
    
    // Type checking
    if (left->type == STRING || right->type == STRING) {
        reportError(ERROR_TYPE, getCurrentLine(), 
                   "Cannot perform arithmetic operations with strings");
    }

    if (left->type == BOOLEAN || right->type == BOOLEAN) {
        reportError(ERROR_TYPE, getCurrentLine(), 
                   "Cannot perform arithmetic operations with booleans");
    }
    
    float leftVal = (left->type == FLOAT) ? left->value.floatValue : (float)left->value.intValue;
    float rightVal = (right->type == FLOAT) ? right->value.floatValue : (float)right->value.intValue;
    
    result.type = FLOAT;
    result.name = NULL;
    result.isConstant = 0;

    if (strcmp(operator, "**") == 0) {
        result.value.floatValue = pow(leftVal, rightVal);
    } else if (strcmp(operator, "//") == 0) {
        if (rightVal == 0) {
            reportError(ERROR_DIV_BY_ZERO, getCurrentLine(), "Division by zero");
        }
        result.type = INT;
        result.value.intValue = (int)(leftVal / rightVal);
    } else if (strcmp(operator, "%") == 0) {
        if (rightVal == 0) {
            reportError(ERROR_DIV_BY_ZERO, getCurrentLine(), "Modulo by zero");
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
                    reportError(ERROR_DIV_BY_ZERO, getCurrentLine(), "Division by zero");
                }
                result.value.floatValue = leftVal / rightVal;
                break;
            default:
                reportError(ERROR_SYNTAX, getCurrentLine(), "Unknown operator '%s'", operator);
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
