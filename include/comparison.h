#ifndef COMPARISON_H
#define COMPARISON_H

#include "types.h"

/**
 * @file comparison.h
 * @brief Comparison operations for Noviq interpreter
 */

/**
 * @brief Perform a comparison operation
 * @param left Left operand
 * @param right Right operand
 * @param operator The comparison operator (>, <, >=, <=, ==)
 * @return Result of the comparison
 */
Variable performComparison(Variable *left, Variable *right, const char *operator);

/**
 * @brief Check if a string is a comparison operator
 * @param str The string to check
 * @return 1 if it's a comparison operator, 0 otherwise
 */
int isComparisonOperator(const char *str);

#endif // COMPARISON_H
