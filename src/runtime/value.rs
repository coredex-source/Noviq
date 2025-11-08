// runtime/value.rs
// Runtime value type definitions.
// Defines the types of values that can exist during program execution.

/// Runtime values for Noviq
/// 
/// Represents values that can be computed and manipulated at runtime.

#[derive(Debug, Clone, PartialEq)]
pub enum Value {
    String(String),
    Null,
}

impl std::fmt::Display for Value {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            Value::String(s) => write!(f, "{}", s),
            Value::Null => write!(f, "null"),
        }
    }
}
