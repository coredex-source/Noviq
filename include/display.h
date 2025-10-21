#ifndef DISPLAY_H
#define DISPLAY_H

/**
 * @file display.h
 * @brief Display/output functions for Noviq interpreter
 */

/**
 * @brief Display a string
 * @param text The text to display
 */
void display(const char *text);

/**
 * @brief Display an integer
 * @param value The integer to display
 */
void displayInt(int value);

/**
 * @brief Display a float
 * @param value The float to display
 */
void displayFloat(float value);

/**
 * @brief Display formatted text with variable substitution
 * @param format The format string containing %var1, %var2, etc.
 * @param vars Array of variable names/expressions to substitute
 * @param varCount Number of variables
 */
void displayFormatted(const char *format, char **vars, int varCount);

#endif // DISPLAY_H
