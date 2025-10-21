#include "logical.h"
#include <string.h>
#include <stdlib.h>

int isLogicalOperator(const char *str) {
    return (strcmp(str, "AND") == 0 || 
            strcmp(str, "OR") == 0 || 
            strcmp(str, "NOT") == 0 ||
            strcmp(str, "&&") == 0 ||
            strcmp(str, "||") == 0 ||
            strcmp(str, "!") == 0);
}

Variable performLogicalOperation(Variable *left, Variable *right, const char *operator) {
    Variable result;
    result.type = BOOLEAN;
    result.name = NULL;
    result.isConstant = 0;

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

    if (strcmp(operator, "AND") == 0 || strcmp(operator, "&&") == 0) {
        result.value.boolValue = leftBool && rightBool;
    } else if (strcmp(operator, "OR") == 0 || strcmp(operator, "||") == 0) {
        result.value.boolValue = leftBool || rightBool;
    } else if (strcmp(operator, "NOT") == 0 || strcmp(operator, "!") == 0) {
        result.value.boolValue = !leftBool;
    }

    return result;
}
