#ifndef VARIABLES_H
#define VARIABLES_H

#include <stddef.h>
#include "types.h"

/**
 * @file variables.h
 * @brief Variable management for Noviq interpreter
 */

/**
 * @brief Find a variable by name
 * @param name The name of the variable
 * @return Pointer to the variable if found, NULL otherwise
 */
Variable *findVariable(const char *name);

/**
 * @brief Add or update a variable
 * @param name The name of the variable
 * @param type The type of the variable
 * @param value Pointer to the value
 * @param isConstant Whether the variable is a constant
 */
void addVariable(const char *name, VarType type, void *value, int isConstant);

/**
 * @brief Free all allocated variables
 */
void freeAllVariables(void);

/**
 * @brief Free a single variable
 * @param var The variable to free
 */
void freeVariable(Variable *var);

/**
 * @brief Get the count of variables
 * @return The number of variables
 */
size_t getVariableCount(void);

#endif // VARIABLES_H
