# Noviq Examples

This directory contains example Noviq programs (`.nvq` files).

## Running Examples

```bash
# Run an example file
noviq examples/hello.nvq

# Or with cargo
cargo run -- examples/hello.nvq
```

## Available Examples

- `hello.nvq` - Basic hello world program
- `test.nvq` - Simple test file
- `variables.nvq` - Variable declaration and usage
- `quotes.nvq` - Single and double quote strings
- `showcase.nvq` - Comprehensive feature demonstration
- `interpolation_demo.nvq` - Complete interpolation demo

## Features Demonstrated

- Comments (lines starting with `#`)
- Print statements with `print()`
- String literals (double and single quotes)
- String interpolation with `{variable_name}` syntax
- Variable declarations with `let`
- Numbers (integers and floats)
- Booleans (true/false)
- Escaped braces in strings (`{{` and `}}`)
