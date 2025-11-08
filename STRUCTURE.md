# Noviq Project Structure

This document describes the organization of the Noviq interpreter codebase.

## Overview

Noviq is an interpreted programming language written in Rust. The project is organized as a library (`lib.rs`) with a minimal binary entry point (`main.rs`).

## Directory Structure

```
Noviq/
├── src/
│   ├── main.rs                  # Binary entry point
│   ├── lib.rs                   # Library root
│   ├── utils/                   # Utilities
│   │   ├── mod.rs               # Utils module exports
│   │   ├── version.rs           # Version management
│   │   └── tests.rs             # Integration tests
│   ├── frontend/                # Lexer and Parser
│   │   ├── mod.rs               # Frontend module exports
│   │   ├── token.rs             # Token definitions
│   │   ├── ast.rs               # AST definitions
│   │   ├── lexer/               # Tokenization
│   │   │   ├── mod.rs           # Lexer main logic
│   │   │   └── reader.rs        # Character reading utilities
│   │   └── parser/              # AST generation
│   │       ├── mod.rs           # Parser main logic
│   │       ├── expr.rs          # Expression parsing
│   │       └── stmt.rs          # Statement parsing
│   └── runtime/                 # Interpreter and execution
│       ├── mod.rs               # Runtime module exports
│       ├── value.rs             # Runtime value types
│       ├── builtins/            # Built-in functions
│       │   ├── mod.rs           # Builtin router
│       │   └── log.rs           # log() function
│       └── interpreter/         # Execution engine
│           ├── mod.rs           # Interpreter main logic
│           ├── expr.rs          # Expression evaluation
│           └── stmt.rs          # Statement execution
├── examples/                    # Example Noviq programs
│   ├── README.md
│   ├── hello.nvq
│   └── test.nvq
├── Cargo.toml
├── README.md
├── STRUCTURE.md
└── build.sh/bat/ps1
```

## Module Organization

### `main.rs`
The binary entry point. Kept minimal - only initializes and calls into the library.
Handles command-line arguments:
- No args: Shows version info and usage
- `--help` / `-h`: Shows help message
- `--version` / `-v`: Shows version
- `<file.nvq>`: Reads and executes a Noviq file

### `lib.rs`
The library root that:
- Declares all modules
- Re-exports public API
- Provides documentation for the crate

### `utils/`
Utility modules:
- `version.rs` - Version string generation (nebula-X.Y.Z or pulsar builds)
- `tests.rs` - Integration tests

### `frontend/`
Tokenization and parsing:
- `token.rs` - Token type definitions
- `ast.rs` - AST node definitions (Expr, Stmt)
- `lexer/` - Tokenizer implementation
  - `mod.rs` - Main lexer logic
  - `reader.rs` - Character reading and string/identifier extraction
- `parser/` - Parser implementation
  - `mod.rs` - Main parser logic
  - `expr.rs` - Expression parsing
  - `stmt.rs` - Statement parsing

### `runtime/`
Execution and built-in functions:
- `value.rs` - Runtime value types
- `builtins/` - Built-in function implementations
  - `mod.rs` - Builtin function router
  - `log.rs` - The `log()` function
- `interpreter/` - Execution engine
  - `mod.rs` - Main interpreter logic
  - `expr.rs` - Expression evaluator
  - `stmt.rs` - Statement executor

### `tests.rs`
Integration tests that verify:
- Example files exist and are readable
- File syntax is correct
- Core functionality works as expected

### `examples/`
Example Noviq programs (`.nvq` extension):
- `hello.nvq` - Basic hello world with log statements
- `test.nvq` - Simple log test
- Each example demonstrates language features

## Current Features

The interpreter currently supports:
- **Comments**: Lines starting with `#`
- **String literals**: Double-quoted strings with escape sequences
- **Function calls**: `log("message")` - prints to stdout

## Module Organization

### Frontend (Parsing)
```
frontend/
├── token.rs           - Token enum (Identifier, String, LeftParen, etc.)
├── ast.rs             - AST nodes (Expr, Stmt)
├── lexer/
│   ├── mod.rs         - Main tokenization logic
│   └── reader.rs      - Character-level reading (skip whitespace, read strings, etc.)
└── parser/
    ├── mod.rs         - Main parsing logic
    ├── expr.rs        - Expression parser
    └── stmt.rs        - Statement parser
```

### Runtime (Execution)
```
runtime/
├── value.rs           - Value enum (String, Null)
├── builtins/
│   ├── mod.rs         - Routes builtin calls by name
│   └── log.rs         - log() implementation
└── interpreter/
    ├── mod.rs         - Main interpreter
    ├── expr.rs        - Expression evaluator
    └── stmt.rs        - Statement executor
```

### Utils
```
utils/
├── version.rs         - Version string generation
└── tests.rs           - Integration tests
```

## Building

```bash
# Debug build (includes pulsar timestamp)
cargo build

# Release build
cargo build --release

# Snapshot build (optimized with pulsar timestamp and git commit hash)
SNAPSHOT=1 cargo build --profile=snapshot
```

Version format:
- **Debug**: `nebula-X.Y.Z-pulsar.YYMMDD`
- **Snapshot**: `nebula-X.Y.Z-pulsar.YYMMDD.GITHASH`
- **Release**: `nebula-X.Y.Z`

The git commit hash ensures snapshot builds have a unique identifier that stays the same unless changes are committed.

## Testing

```bash
# Run all tests
cargo test

# Run a specific test
cargo test test_example_files_exist

# Run tests with output
cargo test -- --nocapture
```

## Running Examples

```bash
# Run with cargo
cargo run -- examples/hello.nvq

# Or run the binary directly
./target/debug/noviq examples/hello.nvq

# Show help
./target/debug/noviq --help

# Show version
./target/debug/noviq --version
```

## Design Principles

1. **Separation of Concerns**: Each module has a single, well-defined purpose
2. **Library-First**: Core functionality in `lib.rs`, binary is just a thin wrapper
3. **Testability**: Each module includes its own tests
4. **Documentation**: All public items are documented
5. **Extensibility**: Easy to add new modules without modifying existing code

## Adding New Features

To add a new feature:

1. Create a new module file in the appropriate directory
2. Add module declaration to `lib.rs` or the parent module
3. Implement the feature with tests
4. Update documentation
5. Re-export public items if needed

Example:
```rust
// src/builtins/math.rs
pub fn add(a: i64, b: i64) -> i64 { a + b }

// src/builtins/mod.rs
pub mod math;
pub use math::add;
```
