```diff

@@ The language is currently in Alpha development stages. @@

@@ A lot of missing features and bugs are to be expected. @@
```

# Noviq

# Noviq- The name Noviq is make up of two words: Nova(which means new) and unique.

- So, Nova + unique = Noviq.

**Noviq** = **Nova** (new) + **unique**- Noviq is just a simple interpreter based programming language written in C.



A simple, interpreted programming language written in C with a focus on clarity and ease of use.## Wiki

[Click Here To Go To The Wiki](https://coredex-source.github.io/Noviq-site/wiki/introduction.html)

## Features

## Building and using

✅ **Dynamic Typing** - Integers, floats, strings, and booleans  ### Requirements:

✅ **Arithmetic Operations** - `+`, `-`, `*`, `/`, `%`, `**` (power), `//` (integer division)    - GCC (Only GCC on windows.)

✅ **Logical Operations** - `AND`, `OR`, `NOT` (also `&&`, `||`, `!`)    - make

✅ **Comparison Operators** - `>`, `<`, `>=`, `<=`, `==`  ### Build using:

✅ **Control Flow** - `if`/`else` statements with nesting support  - MacOS/Linux

✅ **Constants** - Immutable values with `const` keyword  ```

✅ **User Input** - Interactive programs with `input()`  make all

✅ **String Formatting** - Variable substitution with `%var1`, `%var2`, etc.```


## Documentation```

gcc -o noviq.exe noviq.c lexer/lexer_interpret.c lexer/lexer_display.c -lm

📚 [Language Wiki](https://coredex-source.github.io/Noviq-site/wiki/introduction.html)  ```

📖 [Language Reference](docs/language_reference.md)### Run using:


## Project Structure
```
├── src/
│   ├── main.c                  # Entry pointnoviq.exe -e filename.nvq
│   ├── interpreter/            # Core interpreter
│   │   ├── interpreter.c       # Command interpretation
│   │   ├── variables.c         # Variable management
│   │   └── expressions.c       # Expression evaluation
│   ├── operators/              # Operator implementations
│   │   ├── arithmetic.c        # Arithmetic operations
│   │   ├── logical.c           # Logical operations
│   │   └── comparison.c        # Comparison operations
│   ├── statements/             # Statement handlers
│   │   ├── display.c           # Output functionality
│   │   ├── input.c             # Input handling
│   │   └── control_flow.c      # If/else statements
│   └── utils/
│       └── error.c             # Error reporting
├── include/                     # Header files
├── examples/                    # Example programs
├── tests/                       # Test files
├── docs/                        # Documentation
├── CMakeLists.txt              # CMake build configuration
├── Makefile                    # Make build configuration
└── README.md
```

## Quick Start

### Requirements

- **GCC** or compatible C compiler
- **Make** (optional, for Makefile build)
- **CMake** 3.10+ (optional, for CMake build)

### Build Options

#### Option 1: Using Make (Recommended for Unix-like systems)

```bash
make all
```

#### Option 2: Using CMake (Cross-platform)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

#### Option 3: Direct compilation (Windows)

```cmd
gcc -o noviq.exe -Iinclude src\main.c src\interpreter\*.c src\operators\*.c src\statements\*.c src\utils\*.c -lm
```

### Run

```bash
# Unix/Linux/macOS
./noviq -e examples/hello_world.nvq

# Windows
noviq.exe -e examples\hello_world.nvq
```

### Command-Line Options

```
noviq [options]

Options:
  -e <filename>    Execute a Noviq script file (.nvq)
  --help           Display help message
  --version        Display version information
```

## Examples

### Hello World

```noviq
display("Hello, World!")
```

### Variables and Arithmetic

```noviq
x = 10
y = 5
result = x + y
display("Result: %var1", result)
```

### Control Flow

```noviq
age = 20
if(age >= 18){
    display("You are an adult")
} else {
    display("You are a minor")
}
```

### Constants

```noviq
const PI = 3.14159
radius = 5
area = PI * radius * radius
display("Area: %var1", area)
```

More examples available in the `examples/` directory.

## Development

### Building from Source

1. Clone the repository
```bash
git clone https://github.com/coredex-source/Noviq.git
cd Noviq
```

2. Build using your preferred method
```bash
make all
```

3. Test the build
```bash
./noviq -e examples/hello_world.nvq
```

### Clean Build

```bash
make clean
make all
```

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

GPL-3.0 - See [LICENSE](LICENSE) for details.

## Version

Current version: **prealpha-v2.3.0**

---

*Note: This project is in pre-alpha development. Features may change, and bugs are expected.*
