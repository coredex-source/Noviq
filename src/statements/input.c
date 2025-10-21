#include "input.h"
#include "variables.h"
#include "expressions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processInput(const char *prompt, const char *varName) {
    char buffer[1024];
    
    // Print the prompt
    printf("%s", prompt);
    fflush(stdout);
    
    // Get input
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Remove trailing newline
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
        
        // Try to parse as number first
        char *endptr;
        long intValue = strtol(buffer, &endptr, 10);
        
        if (*endptr == '\0') {
            // It's an integer
            int value = (int)intValue;
            addVariable(varName, INT, &value, 0);
        } else if (isFloat(buffer)) {
            // It's a float
            float value = parseFloat(buffer);
            addVariable(varName, FLOAT, &value, 0);
        } else if (strcmp(buffer, "true") == 0) {
            // It's boolean true
            int value = 1;
            addVariable(varName, BOOLEAN, &value, 0);
        } else if (strcmp(buffer, "false") == 0) {
            // It's boolean false
            int value = 0;
            addVariable(varName, BOOLEAN, &value, 0);
        } else {
            // Treat as string by default
            addVariable(varName, STRING, buffer, 0);
        }
    }
}

void handleInputCommand(const char *args) {
    char *argsTemp = strdup(args);
    char **prompts = NULL;
    char **variables = NULL;
    int count = 0;
    int maxVars = 10;  // Maximum variables to handle
    
    prompts = malloc(maxVars * sizeof(char*));
    variables = malloc(maxVars * sizeof(char*));
    
    // Initialize prompts to NULL
    for (int i = 0; i < maxVars; i++) {
        prompts[i] = NULL;
    }
    
    char *token = strtok(argsTemp, ",");
    while (token != NULL && count < maxVars) {
        // Skip leading whitespace
        while (*token == ' ') token++;
        
        if (token[0] == '"' || token[0] == '\'') {
            // This is a prompt
            size_t len = strlen(token);
            token[len-1] = '\0';  // Remove closing quote
            prompts[count] = strdup(token + 1);
        } else {
            // This is a variable
            variables[count] = strdup(token);
            count++;
        }
        token = strtok(NULL, ",");
    }
    
    // Process all variables
    for (int i = 0; i < count; i++) {
        // If we have a prompt for this variable, use it
        // Otherwise use a modified version of the first prompt
        if (prompts[i]) {
            processInput(prompts[i], variables[i]);
        } else if (i > 0 && prompts[0]) {
            // For subsequent variables using the same prompt, just show ": "
            processInput(": ", variables[i]);
        } else {
            processInput(prompts[0], variables[i]);
        }
    }
    
    // Cleanup
    for (int i = 0; i < count; i++) {
        if (prompts[i]) free(prompts[i]);
        if (variables[i]) free(variables[i]);
    }
    free(prompts);
    free(variables);
    free(argsTemp);
}
