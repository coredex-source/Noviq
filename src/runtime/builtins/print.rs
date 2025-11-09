// runtime/builtins/print.rs
// Implementation of the print() built-in function.
// Prints values to stdout, separated by spaces.

use crate::frontend::ast::Expr;
use crate::runtime::value::Value;

/// Implements the `print` builtin.
///
/// Evaluates each argument using the provided evaluator and prints them separated by spaces.
/// Supports string interpolation with {variable_name} syntax.
pub fn call<F>(args: Vec<Expr>, eval: &mut F) -> Result<Value, String>
where
    F: FnMut(Expr) -> Result<Value, String>,
{
    if args.is_empty() {
        println!();
        return Ok(Value::Null);
    }

    for (i, arg) in args.into_iter().enumerate() {
        let v = eval(arg)?;
        if i > 0 {
            print!(" ");
        }
        
        // Handle string interpolation
        match v {
            Value::String(s) => {
                let interpolated = interpolate_string(&s, eval)?;
                print!("{}", interpolated);
            }
            _ => print!("{}", v),
        }
    }
    println!();

    Ok(Value::Null)
}

/// Interpolates variables in a string using {variable_name} syntax
fn interpolate_string<F>(s: &str, eval: &mut F) -> Result<String, String>
where
    F: FnMut(Expr) -> Result<Value, String>,
{
    let mut result = String::new();
    let mut chars = s.chars().peekable();
    
    while let Some(ch) = chars.next() {
        if ch == '{' {
            // Check if it's an escaped brace
            if chars.peek() == Some(&'{') {
                chars.next(); // Skip the second '{'
                result.push('{');
                continue;
            }
            
            // Read variable name
            let mut var_name = String::new();
            let mut found_closing = false;
            
            while let Some(&next_ch) = chars.peek() {
                if next_ch == '}' {
                    chars.next(); // Consume '}'
                    found_closing = true;
                    break;
                } else if next_ch.is_alphanumeric() || next_ch == '_' {
                    var_name.push(chars.next().unwrap());
                } else {
                    return Err(format!("Invalid character in variable name: {}", next_ch));
                }
            }
            
            if !found_closing {
                return Err("Unclosed variable interpolation: missing '}'".to_string());
            }
            
            if var_name.is_empty() {
                return Err("Empty variable name in interpolation".to_string());
            }
            
            // Evaluate the variable
            let value = eval(Expr::Identifier(var_name.clone()))?;
            result.push_str(&value.to_string());
        } else if ch == '}' {
            // Check for escaped closing brace
            if chars.peek() == Some(&'}') {
                chars.next();
                result.push('}');
            } else {
                // Unmatched closing brace
                result.push(ch);
            }
        } else {
            result.push(ch);
        }
    }
    
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::collections::HashMap;
    
    // Helper function to create a test evaluator
    fn create_test_eval(vars: HashMap<String, Value>) -> impl FnMut(Expr) -> Result<Value, String> {
        let variables = vars;
        move |expr: Expr| -> Result<Value, String> {
            match expr {
                Expr::String(s) => Ok(Value::String(s)),
                Expr::Number(n) => Ok(Value::Number(n)),
                Expr::Boolean(b) => Ok(Value::Boolean(b)),
                Expr::Identifier(name) => {
                    variables.get(&name).cloned().ok_or_else(|| format!("Undefined variable: {}", name))
                }
                _ => Err("Unsupported expression".to_string()),
            }
        }
    }
    
    #[test]
    fn test_interpolate_string_simple() {
        let mut vars = HashMap::new();
        vars.insert("name".to_string(), Value::String("Alice".to_string()));
        
        let mut eval = create_test_eval(vars);
        let result = interpolate_string("Hello {name}", &mut eval).unwrap();
        assert_eq!(result, "Hello Alice");
    }
    
    #[test]
    fn test_interpolate_string_number() {
        let mut vars = HashMap::new();
        vars.insert("age".to_string(), Value::Number(25.0));
        
        let mut eval = create_test_eval(vars);
        let result = interpolate_string("Age: {age}", &mut eval).unwrap();
        assert_eq!(result, "Age: 25");
    }
    
    #[test]
    fn test_interpolate_string_boolean() {
        let mut vars = HashMap::new();
        vars.insert("flag".to_string(), Value::Boolean(true));
        
        let mut eval = create_test_eval(vars);
        let result = interpolate_string("Status: {flag}", &mut eval).unwrap();
        assert_eq!(result, "Status: true");
    }
    
    #[test]
    fn test_interpolate_string_multiple() {
        let mut vars = HashMap::new();
        vars.insert("name".to_string(), Value::String("Bob".to_string()));
        vars.insert("age".to_string(), Value::Number(30.0));
        
        let mut eval = create_test_eval(vars);
        let result = interpolate_string("{name} is {age} years old", &mut eval).unwrap();
        assert_eq!(result, "Bob is 30 years old");
    }
    
    #[test]
    fn test_interpolate_string_no_variables() {
        let vars = HashMap::new();
        let mut eval = create_test_eval(vars);
        let result = interpolate_string("Just plain text", &mut eval).unwrap();
        assert_eq!(result, "Just plain text");
    }
    
    #[test]
    fn test_interpolate_string_escaped_braces() {
        let vars = HashMap::new();
        let mut eval = create_test_eval(vars);
        let result = interpolate_string("Literal {{braces}}", &mut eval).unwrap();
        assert_eq!(result, "Literal {braces}");
    }
    
    #[test]
    fn test_interpolate_string_escaped_closing() {
        let vars = HashMap::new();
        let mut eval = create_test_eval(vars);
        let result = interpolate_string("Test }}", &mut eval).unwrap();
        assert_eq!(result, "Test }");
    }
    
    #[test]
    fn test_interpolate_string_undefined_variable() {
        let vars = HashMap::new();
        let mut eval = create_test_eval(vars);
        let result = interpolate_string("Hello {undefined}", &mut eval);
        assert!(result.is_err());
        assert!(result.unwrap_err().contains("Undefined variable"));
    }
    
    #[test]
    fn test_interpolate_string_empty_variable() {
        let vars = HashMap::new();
        let mut eval = create_test_eval(vars);
        let result = interpolate_string("Empty {}", &mut eval);
        assert!(result.is_err());
        assert!(result.unwrap_err().contains("Empty variable name"));
    }
    
    #[test]
    fn test_interpolate_string_unclosed_brace() {
        let vars = HashMap::new();
        let mut eval = create_test_eval(vars);
        let result = interpolate_string("Unclosed {name", &mut eval);
        assert!(result.is_err());
        assert!(result.unwrap_err().contains("missing '}'"));
    }
}
