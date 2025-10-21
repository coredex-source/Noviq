#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "types.h"

/**
 * @file arithmetic.h
 * @brief Arithmetic operations for Noviq interpreter
 */

/**
 * @brief Perform an arithmetic operation
 * @param left Left operand
 * @param right Right operand
 * @param operator The arithmetic operator (+, -, *, /, %, **, //)
 * @return Result of the operation
 */
Variable performArithmeticOperation(Variable *left, Variable *right, const char *operator);

/**
 * @brief Check if a character is an operator
 * @param c The character to check
 * @return 1 if it's an operator, 0 otherwise
 */
int isOperator(char c);

#endif // ARITHMETIC_H
