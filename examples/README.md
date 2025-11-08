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

### `hello.nvq`
Basic hello world program demonstrating:
- Print statements with `log()`
- Comments with `#`
- String literals

### `test.nvq`
Basic test file for development

## Current Language Features

- **Comments**: `# This is a comment`
- **String literals**: `"Hello, World!"`
- **Function calls**: `log("message")`
