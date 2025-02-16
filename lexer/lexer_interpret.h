#ifndef LEXER_INTERPRET_H
#define LEXER_INTERPRET_H

typedef enum { INT, STRING, FLOAT, BOOLEAN } VarType;

typedef struct {
    char *name;
    VarType type;
    int isConstant;  // Add this field
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

// Add these function declarations
int isLogicalOperator(const char *str);
Variable performLogicalOperation(Variable *left, Variable *right, const char *operator);

// Add these function declarations
int isComparisonOperator(const char *str);
Variable performComparison(Variable *left, Variable *right, const char *operator);

// Add input handling functions
void handleInputCommand(const char *args);
void processInput(const char *prompt, const char *varName);

// Add if statement parsing declarations
void executeIfBlock(const char *condition, const char *block);
int evaluateCondition(const char *condition);

#endif // LEXER_INTERPRET_H
