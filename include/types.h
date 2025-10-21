#ifndef TYPES_H
#define TYPES_H

/**
 * @file types.h
 * @brief Type definitions for Noviq interpreter
 */

typedef enum { 
    INT, 
    STRING, 
    FLOAT, 
    BOOLEAN 
} VarType;

typedef struct {
    char *name;
    VarType type;
    int isConstant;
    union {
        int intValue;
        char *stringValue;
        float floatValue;
        int boolValue;
    } value;
} Variable;

#endif // TYPES_H
