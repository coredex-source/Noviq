// runtime/mod.rs
// Runtime module declaration.
// Exports the interpreter, value types, and built-in functions.

/// Runtime - Interpreter and Execution
/// 
/// This module handles execution of parsed Noviq programs.

pub mod value;
pub mod builtins;
pub mod interpreter;

pub use value::Value;
pub use interpreter::Interpreter;
