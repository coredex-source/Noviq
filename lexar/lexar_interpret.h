#ifndef LEXAR_INTERPRET_H
#define LEXAR_INTERPRET_H

typedef enum { INT, STRING, FLOAT, BOOLEAN } VarType;

typedef struct {
    char *name;
    VarType type;
    union {
        int intValue;
        char *stringValue;
        float floatValue;
        int boolValue;  // Using int for boolean (0/1)
    } value;
} Variable;

void interpretCommand(const char *command, int lineNumber);
Variable *findVariable(const char *name);

// Add these helper function declarations
int isFloat(const char *str);
float parseFloat(const char *str);

// Add boolean helper function declaration
int isBoolean(const char *str);

#endif // LEXAR_INTERPRET_H
