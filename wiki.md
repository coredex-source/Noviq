# Noviq Language Wiki

**Version**: 0.1.1 (Pre-Alpha - Nebula)  
**Last Updated**: November 8, 2025

## Table of Contents

1. [Overview](#overview)
2. [Language Features](#language-features)
3. [Syntax Guide](#syntax-guide)
4. [Built-in Functions](#built-in-functions)
5. [Data Types](#data-types)
6. [Examples](#examples)
7. [Building & Running](#building--running)
8. [Project Structure](#project-structure)
9. [Tooling](#tooling)
10. [Development Status](#development-status)

---

## Overview

Noviq is a simple, interpreted programming language written in Rust with a focus on clarity and ease of use. The language aims to eventually become self-hosted (written in Noviq itself) and transition from an interpreted to a compiled language.

### Key Characteristics

- **Interpreted**: Currently runs as an interpreter
- **Dynamically Typed**: Variables infer their type from values
- **Simple Syntax**: Easy to read and write
- **UTF-8 Support**: Full Unicode support in strings
- **Cross-Platform**: Runs on Linux, macOS, and Windows

### Version Scheme

- **Pre-alpha (Nebula)**: `nebula-x.x.x` - Current stage
- **Alpha (Protostar)**: `protostar-x.x.x` - Planned
- **Beta (Nova)**: `nova-x.x.x` - Planned
- **Release (Supernova)**: `supernova-x.x.x` - Planned
- **Snapshots (Pulsar)**: `type-x.x.x-pulsar.YYMMDD.GITHASH` - Development builds

---

## Language Features

### Currently Implemented

- ✅ **Comments**: Single-line comments with `#`
- ✅ **Variables**: Declaration with `let` keyword
- ✅ **String Literals**: Both single (`'`) and double (`"`) quotes
- ✅ **String Interpolation**: Variables in strings with `{variable_name}` syntax
- ✅ **Numbers**: Integers and floating-point numbers
- ✅ **Booleans**: `true` and `false` literals
- ✅ **Print Function**: `print()` for output
- ✅ **Variable References**: Use variables in expressions

### Planned Features

- ⏳ Arithmetic operations (+, -, *, /, %)
- ⏳ Comparison operators (==, !=, <, >, <=, >=)
- ⏳ Logical operators (and, or, not)
- ⏳ Control flow (if/else)
- ⏳ Loops (for, while)
- ⏳ Functions (user-defined)
- ⏳ Arrays/Lists
- ⏳ Hash maps/Dictionaries
- ⏳ String interpolation
- ⏳ File I/O
- ⏳ Module system
- ⏳ Error handling

---

## Syntax Guide

### Comments

```noviq
# This is a single-line comment
# Comments start with # and go to the end of the line
```

### Variable Declaration

```noviq
# Syntax: let variable_name = value
let message = "Hello, World!"
let count = 42
let price = 19.99
let is_valid = true
```

**Rules**:
- Variables must be declared with `let`
- Variable names can contain letters, numbers, and underscores
- Variable names must start with a letter or underscore
- Variable names are case-sensitive

### Data Types

#### Strings

```noviq
# Double quotes
let name = "Alice"

# Single quotes
let greeting = 'Hello'

# Escape sequences
let multiline = "Line 1\nLine 2\tTabbed"

# String interpolation
let age = 25
print("I am {age} years old")
print("Hello {name}, you are {age}!")
```

**Supported Escape Sequences**:
- `\n` - Newline
- `\t` - Tab
- `\r` - Carriage return
- `\"` - Double quote
- `\'` - Single quote
- `\\` - Backslash

**String Interpolation**:
- Use `{variable_name}` to insert variables into strings
- Works with all value types (strings, numbers, booleans)
- Escape literal braces with `{{` and `}}`

#### Numbers

```noviq
# Integers
let age = 25
let negative = -100

# Floating-point
let pi = 3.14159
let temperature = -273.15
```

#### Booleans

```noviq
let is_active = true
let is_disabled = false
```

### Function Calls

```noviq
# Syntax: function_name(argument)
print("Hello, World!")
print(42)
print(true)

# With variables
let message = "Test"
print(message)
```

---

## Built-in Functions

### `print(value)`

Prints a value to standard output followed by a newline.

**Parameters**:
- `value` - Any value (string, number, boolean, variable)

**Returns**: Nothing (null)

**String Interpolation**:
Strings support variable interpolation using `{variable_name}` syntax.

**Examples**:
```noviq
print("Hello")              # Prints: Hello
print(42)                   # Prints: 42
print(true)                 # Prints: true

let name = "Alice"
print(name)                 # Prints: Alice

# String interpolation
let age = 25
print("I am {age}")         # Prints: I am 25
print("Hello {name}!")      # Prints: Hello Alice!

# Multiple variables
let city = "NYC"
print("{name} lives in {city}")  # Prints: Alice lives in NYC

# Escaped braces
print("Literal {{braces}}")      # Prints: Literal {braces}
```

---

## Data Types

### String

Text enclosed in single or double quotes.

**Properties**:
- Immutable
- UTF-8 encoded
- Supports escape sequences

**Examples**:
```noviq
let text = "Hello, World!"
let char = 'A'
let emoji = "Hello 👋"
```

### Number

Numeric values (integers and floating-point).

**Properties**:
- 64-bit floating-point (f64)
- Can represent integers and decimals
- Scientific notation not yet supported

**Examples**:
```noviq
let integer = 42
let float = 3.14
let negative = -100
let decimal = 0.5
```

### Boolean

True or false values.

**Values**:
- `true`
- `false`

**Examples**:
```noviq
let is_ready = true
let is_complete = false
```

### Null

Represents absence of a value (returned by functions that don't return anything).

**Note**: Currently only used internally, not directly expressible in code.

---

## Examples

### Hello World

```noviq
# hello.nvq
print("Hello, World!")
print("Welcome to Noviq!")
```

**Output**:
```
Hello, World!
Welcome to Noviq!
```

### Variables

```noviq
# variables.nvq
let message = "Hello from a variable!"
let number = 42
let pi = 3.14
let flag = true

print(message)
print(number)
print(pi)
print(flag)
```

**Output**:
```
Hello from a variable!
42
3.14
true
```

### Quote Types

```noviq
# quotes.nvq
let double_quote = "String with double quotes"
let single_quote = 'String with single quotes'

print(double_quote)
print(single_quote)
```

**Output**:
```
String with double quotes
String with single quotes
```

### String Interpolation

```noviq
# interpolation.nvq
let name = "Alice"
let age = 25
let city = "Wonderland"

print("My name is {name}")
print("I am {age} years old")
print("Hello {name}, welcome to {city}!")
```

**Output**:
```
My name is Alice
I am 25 years old
Hello Alice, welcome to Wonderland!
```

```noviq
# showcase.nvq
# Variable declarations with different types
let greeting = "Hello, Noviq!"
let name = 'World'
let answer = 42
let pi = 3.14159
let is_cool = true

# Print variables
print(greeting)
print(name)
print(answer)
print(pi)
print(is_cool)

# Direct values in print
print("Direct string")
print(123)
```

---

## Building & Running

### Prerequisites

- Rust toolchain (1.70+)
- Git (for snapshot builds)

### Using Photon (Recommended)

Photon is the official build tool for Noviq.

```bash
# Build Photon first
cd photon
cargo build --release

# Use Photon to build Noviq
./target/release/photon build              # Debug build
./target/release/photon build release      # Release build
./target/release/photon build snapshot     # Snapshot with git hash

# Install binaries to libs/
./target/release/photon install

# Clean build artifacts
./target/release/photon clean
```

### Using Cargo Directly

```bash
# Debug build
cargo build

# Release build
cargo build --release

# Snapshot build (with version tracking)
SNAPSHOT=1 cargo build --profile=snapshot

# Run tests
cargo test
```

### Running Noviq Programs

```bash
# With cargo
cargo run -- examples/hello.nvq

# With built binary
./target/debug/noviq examples/hello.nvq

# With release binary
./target/release/noviq examples/hello.nvq

# With installed binary
./libs/noviq-nebula-0.1.1-pulsar.YYMMDD.GITHASH-linux-x86_64 examples/hello.nvq
```

### File Extension

Noviq programs use the `.nvq` extension (Noviq source files).

---

## Project Structure

```
Noviq/
├── src/
│   ├── main.rs                  # Entry point
│   ├── lib.rs                   # Library root
│   │
│   ├── utils/                   # Utilities
│   │   ├── mod.rs
│   │   ├── version.rs           # Version management
│   │   └── tests.rs             # Integration tests
│   │
│   ├── frontend/                # Lexer & Parser
│   │   ├── mod.rs
│   │   ├── token.rs             # Token definitions
│   │   ├── ast.rs               # AST definitions
│   │   │
│   │   ├── lexer/               # Tokenization
│   │   │   ├── mod.rs           # Main lexer
│   │   │   └── reader.rs        # Character reading
│   │   │
│   │   └── parser/              # Parsing
│   │       ├── mod.rs           # Main parser
│   │       ├── expr.rs          # Expression parsing
│   │       └── stmt.rs          # Statement parsing
│   │
│   └── runtime/                 # Interpreter
│       ├── mod.rs
│       ├── value.rs             # Value types
│       │
│       ├── builtins/            # Built-in functions
│       │   ├── mod.rs           # Function router
│       │   └── print.rs         # print() implementation
│       │
│       └── interpreter/         # Execution
│           ├── mod.rs           # Main interpreter
│           ├── expr.rs          # Expression evaluator
│           └── stmt.rs          # Statement executor
│
├── examples/                    # Example programs
│   ├── hello.nvq
│   ├── test.nvq
│   ├── variables.nvq
│   ├── quotes.nvq
│   └── showcase.nvq
│
├── photon/                      # Build tool
│   ├── src/
│   │   └── main.rs
│   ├── Cargo.toml
│   ├── README.md
│   ├── QUICKSTART.md
│   ├── OVERVIEW.md
│   └── SCOPE.md
│
├── libs/                        # Built binaries (gitignored)
├── target/                      # Build artifacts (gitignored)
│
├── Cargo.toml                   # Rust package config
├── build.rs                     # Build script (git hash capture)
├── README.md                    # Main documentation
├── STRUCTURE.md                 # Project structure guide
├── ORGANIZATION.md              # Organization details
└── wiki.md                      # This file
```

### Module Organization

- **frontend**: Tokenization and parsing (source → AST)
- **runtime**: Execution (AST → results)
- **utils**: Version management and tests
- **builtins**: Built-in function implementations

---

## Tooling

### Photon

Photon is the official build tool for Noviq, written in Rust.

**Features**:
- Cross-platform (Linux, macOS, Windows)
- Multiple build profiles (debug, release, snapshot)
- Automatic binary installation
- Git integration for version tracking
- Clean interface with status messages

**Commands**:
```bash
photon build [debug|release|snapshot]  # Build Noviq
photon clean                           # Clean artifacts
photon install                         # Install to libs/
photon help                            # Show help
photon version                         # Show version
```

**See**: `photon/README.md` for full documentation

### Build Scripts (Legacy)

Traditional shell scripts are still available:
- `build.sh` - Unix/Linux/macOS
- `build.bat` - Windows (batch)
- `build.ps1` - Windows (PowerShell)

---

## Development Status

### Current Version: 0.1.1 (Pre-Alpha - Nebula)

**What Works**:
- ✅ Basic syntax parsing
- ✅ Variable declarations
- ✅ Print function with string interpolation
- ✅ Comments
- ✅ String, number, and boolean literals
- ✅ Variable references
- ✅ Single and double quoted strings
- ✅ String interpolation with `{variable}` syntax

**Known Limitations**:
- ⚠️ No arithmetic operations yet
- ⚠️ No control flow (if/else, loops)
- ⚠️ No user-defined functions
- ⚠️ No arrays or data structures
- ⚠️ No error handling beyond basic parsing errors
- ⚠️ Variables cannot be reassigned (no mutation)

**Tests**:
- 11 unit tests passing
- Integration tests for example files
- Lexer, parser, and interpreter coverage

### Roadmap

**v0.2.0** - Arithmetic & Operators
- Arithmetic operations (+, -, *, /, %)
- Comparison operators (==, !=, <, >, <=, >=)
- Logical operators (and, or, not)

**v0.3.0** - Control Flow
- If/else statements
- While loops
- For loops

**v0.4.0** - Functions
- Function definitions
- Function calls with parameters
- Return values

**v0.5.0** - Data Structures
- Arrays/Lists
- Hash maps/Dictionaries
- String methods

**v1.0.0** - Language Maturity
- Module system
- File I/O
- Error handling
- Standard library

---

## Language Design Principles

1. **Simplicity**: Easy to learn and use
2. **Clarity**: Code should be readable
3. **Safety**: Minimize runtime errors
4. **Performance**: Fast execution
5. **Extensibility**: Easy to add features

---

## Contributing

Contributions are welcome! The project is in early development and needs:
- Feature implementations
- Bug fixes
- Documentation improvements
- Example programs
- Test coverage

**Repository**: https://github.com/coredex-source/Noviq

---

## Technical Details

### Compilation Pipeline

```
Source Code (.nvq)
    ↓
Lexer (Tokenization)
    ↓
Tokens
    ↓
Parser (AST Generation)
    ↓
Abstract Syntax Tree
    ↓
Interpreter (Execution)
    ↓
Output/Side Effects
```

### Token Types

- Keywords: `let`, `true`, `false`
- Identifiers: Variable and function names
- Literals: Strings, numbers, booleans
- Operators: `=` (assignment)
- Delimiters: `(`, `)`, newline
- Special: Comments (`#`), EOF

### AST Nodes

**Expressions** (`Expr`):
- `String(String)` - String literal
- `Number(f64)` - Number literal
- `Boolean(bool)` - Boolean literal
- `Identifier(String)` - Variable reference
- `Call { name, args }` - Function call

**Statements** (`Stmt`):
- `Let { name, value }` - Variable declaration
- `Expr(Expr)` - Expression statement

### Runtime Values

- `String(String)` - String value
- `Number(f64)` - Numeric value
- `Boolean(bool)` - Boolean value
- `Null` - Null value

---

## FAQ

**Q: Is Noviq ready for production use?**  
A: No, Noviq is in pre-alpha and should only be used for experimentation and learning.

**Q: Can I use Noviq for my project?**  
A: Yes, but be aware of the limitations and expect breaking changes.

**Q: How can I contribute?**  
A: Check the GitHub repository for issues and contribution guidelines.

**Q: Why another programming language?**  
A: Noviq is a learning project focused on understanding language implementation.

**Q: Will there be a standard library?**  
A: Yes, planned for v1.0.0 and beyond.

**Q: Is there IDE support?**  
A: Not yet, but syntax highlighting definitions are planned.

**Q: Can I compile Noviq programs?**  
A: Not yet - currently interpreter-only. Compilation is a future goal.

---

## License

GPL-3.0 - See LICENSE file for details.

---

## Resources

- **Main README**: `README.md`
- **Project Structure**: `STRUCTURE.md`
- **Organization**: `ORGANIZATION.md`
- **Examples**: `examples/README.md`
- **Photon Build Tool**: `photon/README.md`
- **Photon Roadmap**: `photon/SCOPE.md`

---

*Last Updated: November 8, 2025*  
*Current Version: nebula-0.1.1*  
*Status: Pre-Alpha Development*
