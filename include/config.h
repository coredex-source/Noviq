#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file config.h
 * @brief Configuration constants for Noviq interpreter
 */

// Version information
#define NOVIQ_VERSION "1.0.0"
#define NOVIQ_VERSION_STAGE "alpha"
#define NOVIQ_VERSION_FULL NOVIQ_VERSION_STAGE "-v" NOVIQ_VERSION

// Interpreter limits
#define NOVIQ_MAX_LINE_LENGTH 4096
#define NOVIQ_MAX_VARIABLES 1000
#define NOVIQ_MAX_IF_NESTING 32
#define NOVIQ_MAX_VAR_NAME_LENGTH 256
#define NOVIQ_MAX_STRING_LENGTH 1024

// File extension
#define NOVIQ_FILE_EXTENSION ".nvq"

// Buffer sizes
#define BUFFER_SIZE_SMALL 128
#define BUFFER_SIZE_MEDIUM 512
#define BUFFER_SIZE_LARGE 1024

#endif // CONFIG_H
