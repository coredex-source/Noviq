#ifndef LEXAR_INTERPRET_H
#define LEXAR_INTERPRET_H

typedef enum { INT, STRING } VarType;

typedef struct {
    char *name;
    VarType type;
    union {
        int intValue;
        char *stringValue;
    } value;
} Variable;

void interpretCommand(const char *command, int lineNumber);
Variable *findVariable(const char *name);

#endif // LEXAR_INTERPRET_H
