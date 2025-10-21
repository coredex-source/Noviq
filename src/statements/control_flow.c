#include "control_flow.h"
#include "variables.h"
#include "expressions.h"
#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int evaluateCondition(const char *condition) {
    // First try to find it as a boolean variable
    Variable *var = findVariable(condition);
    if (var) {
        if (var->type == BOOLEAN) return var->value.boolValue;
        if (var->type == INT) return var->value.intValue != 0;
        if (var->type == FLOAT) return var->value.floatValue != 0;
        if (var->type == STRING) return strlen(var->value.stringValue) > 0;
    }

    // Try to evaluate as an expression
    Variable *result = evaluateExpression(condition);
    if (result) {
        int value;
        if (result->type == BOOLEAN) value = result->value.boolValue;
        else if (result->type == INT) value = result->value.intValue != 0;
        else if (result->type == FLOAT) value = result->value.floatValue != 0;
        else value = 0;
        free(result);
        return value;
    }

    return 0;
}

void executeIfBlock(const char *condition, const char *block) {
    // Remove any leading/trailing spaces from condition
    while (*condition == ' ' || *condition == '\t') condition++;
    char *conditionEnd = (char *)condition + strlen(condition) - 1;
    while (conditionEnd > condition && (*conditionEnd == ' ' || *conditionEnd == '\t')) conditionEnd--;
    
    // Create temporary condition string
    char *tempCondition = (char *)malloc(conditionEnd - condition + 2);
    strncpy(tempCondition, condition, conditionEnd - condition + 1);
    tempCondition[conditionEnd - condition + 1] = '\0';

    if (evaluateCondition(tempCondition)) {
        // Extract and execute if block
        char *ifContent = strstr(block, "{");
        if (ifContent) {
            // Find the matching closing brace by counting nested braces
            char *endIf = ifContent + 1;
            int braceCount = 1;
            int inString = 0;
            char stringChar = 0;

            while (*endIf && braceCount > 0) {
                // Handle string literals
                if ((*endIf == '"' || *endIf == '\'') && !inString) {
                    inString = 1;
                    stringChar = *endIf;
                } else if (inString && *endIf == stringChar) {
                    inString = 0;
                }
                
                // Only count braces outside of strings
                if (!inString) {
                    if (*endIf == '{') braceCount++;
                    if (*endIf == '}') braceCount--;
                }
                
                // Move to next character if we haven't found the matching brace
                if (braceCount > 0) endIf++;
            }
            
            // Execute if block content
            // Calculate the length of content between { and }
            size_t contentLen = endIf - (ifContent + 1);
            
            char *blockContent = malloc(contentLen + 1);
            strncpy(blockContent, ifContent + 1, contentLen);
            blockContent[contentLen] = '\0';
            
            char *line = strtok(blockContent, "\n");
            while (line) {
                while (*line == ' ' || *line == '\t') line++;
                if (*line) {
                    interpretCommand(line);
                }
                line = strtok(NULL, "\n");
            }
            free(blockContent);
        }
    } else {
        // Find and execute else block if it exists
        char *elseContent = strstr(block, "else");
        // Skip any nested else blocks by ensuring we're at the right nesting level
        while (elseContent) {
            // Count braces between start and else to verify it's the correct else
            const char *temp = block;
            int braceCount = 0;
            while (temp < elseContent) {
                if (*temp == '{') braceCount++;
                if (*temp == '}') braceCount--;
                temp++;
            }
            // If braceCount is 0, we found our else
            if (braceCount == 0) break;
            elseContent = strstr(elseContent + 4, "else");
        }

        if (elseContent) {
            char *elseBlock = strchr(elseContent, '{');
            if (elseBlock) {
                // Find the end of the else block using brace counting
                char *endElse = elseBlock + 1;
                int braceCount = 1;
                while (*endElse && braceCount > 0) {
                    if (*endElse == '{') braceCount++;
                    if (*endElse == '}') braceCount--;
                    if (braceCount > 0) endElse++;
                }

                // Execute else block content
                size_t contentLen = endElse - (elseBlock + 1);
                char *blockContent = malloc(contentLen + 1);
                strncpy(blockContent, elseBlock + 1, contentLen);
                blockContent[contentLen] = '\0';
                
                char *line = strtok(blockContent, "\n");
                while (line) {
                    while (*line == ' ' || *line == '\t') line++;
                    if (*line) {
                        interpretCommand(line);
                    }
                    line = strtok(NULL, "\n");
                }
                free(blockContent);
            }
        }
    }
    
    free(tempCondition);
}
