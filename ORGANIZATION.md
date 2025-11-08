# Noviq Project Organization

This document shows the final modular structure of the Noviq interpreter.

## Directory Structure

```
src/
├── main.rs                      # Entry point
├── lib.rs                       # Library root
│
├── utils/                       # Utilities
│   ├── mod.rs
│   ├── version.rs               # Version management (nebula/pulsar)
│   └── tests.rs                 # Integration tests
│
├── frontend/                    # Source → AST
│   ├── mod.rs
│   ├── token.rs                 # Token definitions
│   ├── ast.rs                   # AST definitions
│   │
│   ├── lexer/                   # Tokenization
│   │   ├── mod.rs               # Main lexer logic
│   │   └── reader.rs            # Character reading
│   │
│   └── parser/                  # Parsing
│       ├── mod.rs               # Main parser logic
│       ├── expr.rs              # Expression parsing
│       └── stmt.rs              # Statement parsing
│
└── runtime/                     # AST → Execution
    ├── mod.rs
    ├── value.rs                 # Value types (String, Null)
    │
    ├── builtins/                # Built-in functions
    │   ├── mod.rs               # Function router
    │   └── log.rs               # log() implementation
    │
    └── interpreter/             # Execution
        ├── mod.rs               # Main interpreter
        ├── expr.rs              # Expression evaluator
        └── stmt.rs              # Statement executor
```

## Module Responsibilities

### Frontend
- **token.rs**: Token enum definitions
- **ast.rs**: AST node definitions (Expr, Stmt)
- **lexer/**: Converts source text → tokens
  - `reader.rs`: Character-level operations
  - `mod.rs`: Tokenization logic
- **parser/**: Converts tokens → AST
  - `expr.rs`: Expression parsing
  - `stmt.rs`: Statement parsing
  - `mod.rs`: Main parser orchestration

### Runtime
- **value.rs**: Runtime value types
- **builtins/**: Built-in functions (each function gets its own file)
  - `log.rs`: The `log()` function
  - `mod.rs`: Routes function calls
- **interpreter/**: Executes AST
  - `expr.rs`: Evaluates expressions
  - `stmt.rs`: Executes statements
  - `mod.rs`: Main interpreter orchestration

### Utils
- **version.rs**: Version string generation
- **tests.rs**: Integration tests

## Benefits of This Structure

1. **Modularity**: Each component has a single responsibility
2. **Extensibility**: Easy to add new builtins (just create a new file in `builtins/`)
3. **Testability**: Each module can be tested independently
4. **Clarity**: Clear separation between parsing and execution
5. **Maintainability**: Small, focused files instead of monoliths

## Adding New Features

### Adding a new builtin function:
1. Create `src/runtime/builtins/myfunction.rs`
2. Implement the function following the pattern in `log.rs`
3. Add it to `src/runtime/builtins/mod.rs`

### Extending the lexer:
- Add character handling to `src/frontend/lexer/reader.rs`
- Add tokenization logic to `src/frontend/lexer/mod.rs`

### Extending the parser:
- Modify `src/frontend/parser/expr.rs` for expressions
- Modify `src/frontend/parser/stmt.rs` for statements
- Update `src/frontend/ast.rs` if new AST nodes are needed

### Extending the interpreter:
- Modify `src/runtime/interpreter/expr.rs` for expression evaluation
- Modify `src/runtime/interpreter/stmt.rs` for statement execution
- Update `src/runtime/value.rs` if new value types are needed
