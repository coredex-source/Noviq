#ifndef CONTROL_FLOW_H
#define CONTROL_FLOW_H

/**
 * @file control_flow.h
 * @brief Control flow statements for Noviq interpreter
 */

/**
 * @brief Evaluate a condition and return boolean result
 * @param condition The condition string to evaluate
 * @return 1 if condition is true, 0 if false
 */
int evaluateCondition(const char *condition);

/**
 * @brief Execute an if/else block
 * @param condition The condition to evaluate
 * @param block The complete block containing if and optional else branches
 */
void executeIfBlock(const char *condition, const char *block);

#endif // CONTROL_FLOW_H
