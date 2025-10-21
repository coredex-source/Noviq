#ifndef INPUT_H
#define INPUT_H

/**
 * @file input.h
 * @brief Input handling functions for Noviq interpreter
 */

/**
 * @brief Process user input and store in a variable
 * @param prompt The prompt to display to the user
 * @param varName The name of the variable to store the input
 */
void processInput(const char *prompt, const char *varName);

/**
 * @brief Handle the input command with multiple variables and prompts
 * @param args The arguments string from the input() command
 */
void handleInputCommand(const char *args);

#endif // INPUT_H
