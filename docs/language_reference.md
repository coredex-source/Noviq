# Noviq Language Reference

**Version:** 2.3.0 (Pre-Alpha)

## Table of Contents

1. [Introduction](#introduction)
2. [Data Types](#data-types)
3. [Variables](#variables)
4. [Constants](#constants)
5. [Operators](#operators)
6. [Control Flow](#control-flow)
7. [Input/Output](#inputoutput)
8. [Comments](#comments)
9. [Error Handling](#error-handling)

## Introduction

Noviq is a dynamically-typed interpreted programming language with a simple syntax designed for clarity and ease of use. Files use the `.nvq` extension.

### Basic Syntax Rules

- Statements are executed line by line
- No semicolons required
- Case-sensitive identifiers
- Whitespace is generally ignored (except in strings)

## Data Types

Noviq supports four primary data types:

### Integer (`INT`)

Whole numbers without decimal points.

```noviq
x = 42
y = -15
z = 0
```

### Float (`FLOAT`)

Numbers with decimal points.

```noviq
pi = 3.14159
temperature = -273.15
ratio = 0.5
```

### String (`STRING`)

Text enclosed in double quotes `"` or single quotes `'`.

```noviq
name = "Alice"
message = 'Hello, World!'
empty = ""
```

### Boolean (`BOOLEAN`)

Logical true/false values.

```noviq
is_active = true
is_complete = false
```

## Variables

Variables are dynamically typed and don't require declaration.

### Variable Assignment

```noviq
x = 10              # Integer
name = "John"       # String
score = 95.5        # Float
active = true       # Boolean
```

### Variable Names

- Must start with a letter
- Can contain letters, numbers, and underscores
- Case-sensitive (`myVar` ≠ `myvar`)
- Cannot be keywords

**Valid names:** `x`, `user_name`, `score2`, `_temp`  
**Invalid names:** `2x`, `my-var`, `for`, `if`

## Constants

Constants are immutable variables declared with the `const` keyword.

```noviq
const PI = 3.14159
const MAX_USERS = 100
const APP_NAME = "Noviq"
```

**Note:** Attempting to modify a constant will result in an error.

```noviq
const X = 10
X = 20  # Error: Cannot modify constant 'X'
```

## Operators

### Arithmetic Operators

| Operator | Description | Example | Result |
|----------|-------------|---------|--------|
| `+` | Addition | `5 + 3` | `8` |
| `-` | Subtraction | `5 - 3` | `2` |
| `*` | Multiplication | `5 * 3` | `15` |
| `/` | Division | `5 / 2` | `2.5` |
| `%` | Modulo | `5 % 2` | `1` |
| `**` | Exponentiation | `2 ** 3` | `8` |
| `//` | Integer Division | `5 // 2` | `2` |

**Examples:**

```noviq
x = 10 + 5          # 15
y = 20 - 8          # 12
z = 4 * 7           # 28
result = 15 / 4     # 3.75
remainder = 17 % 5  # 2
power = 2 ** 8      # 256
int_div = 17 // 5   # 3
```

### Comparison Operators

| Operator | Description | Example | Result |
|----------|-------------|---------|--------|
| `>` | Greater than | `5 > 3` | `true` |
| `<` | Less than | `5 < 3` | `false` |
| `>=` | Greater or equal | `5 >= 5` | `true` |
| `<=` | Less or equal | `5 <= 3` | `false` |
| `==` | Equal | `5 == 5` | `true` |

**Examples:**

```noviq
x = 10
y = 20
result1 = x > y     # false
result2 = x < y     # true
result3 = x >= 10   # true
result4 = y <= 20   # true
result5 = x == y    # false
```

### Logical Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `AND` or `&&` | Logical AND | `true AND false` → `false` |
| `OR` or `||` | Logical OR | `true OR false` → `true` |
| `NOT` or `!` | Logical NOT | `NOT true` → `false` |

**Examples:**

```noviq
a = true
b = false

result1 = a AND b      # false
result2 = a OR b       # true
result3 = NOT a        # false
result4 = a && b       # false (alternative syntax)
result5 = a || b       # true (alternative syntax)
result6 = !a           # false (alternative syntax)
```

### Operator Precedence

1. `**` (Exponentiation)
2. `*`, `/`, `%`, `//` (Multiplication, Division, Modulo)
3. `+`, `-` (Addition, Subtraction)
4. `>`, `<`, `>=`, `<=`, `==` (Comparison)
5. `NOT`, `!` (Logical NOT)
6. `AND`, `&&` (Logical AND)
7. `OR`, `||` (Logical OR)

## Control Flow

### If Statements

Execute code conditionally based on a boolean expression.

**Syntax:**

```noviq
if(condition){
    # code to execute if condition is true
}
```

**Example:**

```noviq
x = 10
if(x > 5){
    display("x is greater than 5")
}
```

### If-Else Statements

Provide an alternative branch when the condition is false.

**Syntax:**

```noviq
if(condition){
    # code if condition is true
} else {
    # code if condition is false
}
```

**Example:**

```noviq
age = 18
if(age >= 18){
    display("You are an adult")
} else {
    display("You are a minor")
}
```

### Nested If Statements

If statements can be nested within each other.

**Example:**

```noviq
x = 10
y = 5

if(x > 5){
    if(y < 10){
        display("Both conditions are true")
    } else {
        display("Only first condition is true")
    }
} else {
    display("First condition is false")
}
```

## Input/Output

### Display Output

The `display()` function outputs text to the console.

**Simple display:**

```noviq
display("Hello, World!")
```

**Display variables:**

```noviq
name = "Alice"
age = 25
display("Name: %var1, Age: %var2", name, age)
```

**Variable placeholders:**
- `%var1` - First variable
- `%var2` - Second variable
- `%var3` - Third variable
- etc.

**Display expressions:**

```noviq
x = 10
y = 5
display("Sum: %var1", x + y)
display("Product: %var1", x * y)
```

### Input

The `input()` function reads user input.

**Single input:**

```noviq
input("Enter your name: ", name)
display("Hello, %var1!", name)
```

**Multiple inputs with one prompt:**

```noviq
input("Enter two numbers: ", x, y)
display("You entered: %var1 and %var2", x, y)
```

**Multiple inputs with separate prompts:**

```noviq
input("Enter x: ", x, "Enter y: ", y)
display("x = %var1, y = %var2", x, y)
```

**Type inference:**

The `input()` function automatically detects the type:
- Integers: `42`, `-15`
- Floats: `3.14`, `-0.5`
- Booleans: `true`, `false`
- Strings: Everything else

## Comments

### Single-Line Comments

Use `#` for single-line comments.

```noviq
# This is a comment
x = 10  # This is also a comment
```

### Multi-Line Comments

Use `##` to start and end multi-line comments.

```noviq
##
This is a multi-line comment
It can span multiple lines
##

x = 5
```

## Error Handling

Noviq provides detailed error messages with line numbers and error types.

### Error Types

1. **Syntax Error** - Invalid syntax
   ```
   Syntax Error on line 5: Missing closing parenthesis
   ```

2. **Type Error** - Invalid type operation
   ```
   Type Error on line 10: Cannot perform arithmetic operations with strings
   ```

3. **Undefined Variable Error** - Variable not found
   ```
   Undefined Variable Error on line 15: Variable 'x' not found
   ```

4. **Constant Modification Error** - Attempting to modify a constant
   ```
   Constant Modification Error on line 20: Cannot modify constant 'PI'
   ```

5. **Division by Zero Error**
   ```
   Division by Zero Error on line 25: Division by zero
   ```

## Best Practices

### 1. Use Meaningful Variable Names

```noviq
# Good
user_age = 25
total_score = 100

# Avoid
x = 25
t = 100
```

### 2. Use Constants for Fixed Values

```noviq
const MAX_ATTEMPTS = 3
const PI = 3.14159
```

### 3. Add Comments

```noviq
# Calculate circle area
const PI = 3.14159
radius = 5
area = PI * radius * radius
```

### 4. Format Output Clearly

```noviq
name = "Alice"
score = 95

display("Student Report")
display("Name: %var1", name)
display("Score: %var1", score)
```

## Limitations (Pre-Alpha)

Current limitations of Noviq:

- No loops (`for`, `while`)
- No functions/procedures
- No arrays or lists
- No file I/O operations
- No string manipulation functions
- No import/module system
- Maximum variable limit: 1000
- Maximum if-nesting: 32 levels
- Maximum line length: 4096 characters

These features are planned for future releases.

## Example Programs

### Calculator

```noviq
input("Enter first number: ", a)
input("Enter second number: ", b)

display("Addition: %var1", a + b)
display("Subtraction: %var1", a - b)
display("Multiplication: %var1", a * b)
display("Division: %var1", a / b)
```

### Grade Calculator

```noviq
input("Enter your score: ", score)

if(score >= 90){
    display("Grade: A")
} else {
    if(score >= 80){
        display("Grade: B")
    } else {
        if(score >= 70){
            display("Grade: C")
        } else {
            if(score >= 60){
                display("Grade: D")
            } else {
                display("Grade: F")
            }
        }
    }
}
```

### Circle Calculator

```noviq
const PI = 3.14159

input("Enter radius: ", radius)

area = PI * radius * radius
circumference = 2 * PI * radius

display("Circle Properties:")
display("Radius: %var1", radius)
display("Area: %var1", area)
display("Circumference: %var1", circumference)
```

## Further Resources

- [Noviq Wiki](https://coredex-source.github.io/Noviq-site/wiki/introduction.html)
- [GitHub Repository](https://github.com/coredex-source/Noviq)
- [Example Programs](../examples/)

---

*Last updated: October 2025*
