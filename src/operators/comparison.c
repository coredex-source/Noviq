#include "comparison.h"
#include "error.h"
#include <string.h>

int isComparisonOperator(const char *str) {
    return (strcmp(str, ">") == 0 || 
            strcmp(str, "<") == 0 || 
            strcmp(str, ">=") == 0 || 
            strcmp(str, "<=") == 0 || 
            strcmp(str, "==") == 0);
}

Variable performComparison(Variable *left, Variable *right, const char *operator) {
    Variable result;
    result.type = BOOLEAN;
    result.name = NULL;
    result.isConstant = 0;

    // Convert operands to comparable values
    float leftVal, rightVal;

    switch(left->type) {
        case INT: leftVal = (float)left->value.intValue; break;
        case FLOAT: leftVal = left->value.floatValue; break;
        case BOOLEAN: leftVal = (float)left->value.boolValue; break;
        default:
            reportError(ERROR_TYPE, getCurrentLine(), "Cannot compare string values");
    }

    switch(right->type) {
        case INT: rightVal = (float)right->value.intValue; break;
        case FLOAT: rightVal = right->value.floatValue; break;
        case BOOLEAN: rightVal = (float)right->value.boolValue; break;
        default:
            reportError(ERROR_TYPE, getCurrentLine(), "Cannot compare string values");
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
