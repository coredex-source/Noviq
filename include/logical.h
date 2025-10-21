#ifndef LOGICAL_H
#define LOGICAL_H

#include "types.h"

/**
 * @file logical.h
 * @brief Logical operations for Noviq interpreter
 */

/**
 * @brief Perform a logical operation
 * @param left Left operand (NULL for NOT operation)
 * @param right Right operand
 * @param operator The logical operator (AND, OR, NOT, &&, ||, !)
 * @return Result of the operation
 */
Variable performLogicalOperation(Variable *left, Variable *right, const char *operator);

/**
 * @brief Check if a string is a logical operator
 * @param str The string to check
 * @return 1 if it's a logical operator, 0 otherwise
 */
int isLogicalOperator(const char *str);

#endif // LOGICAL_H
