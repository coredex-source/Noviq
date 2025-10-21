#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "types.h"

/**
 * @file expressions.h
 * @brief Expression evaluation for Noviq interpreter
 */

/**
 * @brief Evaluate an expression
 * @param expr The expression string to evaluate
 * @return Pointer to the result variable (must be freed by caller)
 */
Variable *evaluateExpression(const char *expr);

/**
 * @brief Check if a string is a float
 * @param str The string to check
 * @return 1 if it's a float, 0 otherwise
 */
int isFloat(const char *str);

/**
 * @brief Parse a string as a float
 * @param str The string to parse
 * @return The float value
 */
float parseFloat(const char *str);

/**
 * @brief Check if a string is an expression (contains operators)
 * @param str The string to check
 * @return 1 if it's an expression, 0 otherwise
 */
int isExpression(const char *str);

#endif // EXPRESSIONS_H
