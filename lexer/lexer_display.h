#ifndef LEXER_DISPLAY_H
#define LEXER_DISPLAY_H

void display(const char *text);
void displayInt(int value);
void displayFloat(float value);  // Add this declaration
void displayFormatted(const char *format, char **vars, int varCount);
char* createFormattedString(const char *format, char **vars, int varCount);

#endif // LEXER_DISPLAY_H