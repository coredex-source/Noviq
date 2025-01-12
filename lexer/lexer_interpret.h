#ifndef LEXER_INTERPRET_H
#define LEXER_INTERPRET_H

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

// Add global line number
extern int currentLineNumber;

void interpretCommand(const char *command, int lineNumber);
Variable *findVariable(const char *name);

// Add these helper function declarations
int isFloat(const char *str);
float parseFloat(const char *str);

// Add boolean helper function declaration
int isBoolean(const char *str);

// Add arithmetic operation helper functions
int isOperator(char c);
Variable performOperation(Variable *left, Variable *right, const char *operator);
Variable *evaluateExpression(const char *expr);

#endif // LEXER_INTERPRET_H
