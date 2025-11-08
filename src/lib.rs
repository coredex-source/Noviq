// lib.rs
// Main library file for Noviq.
// Declares all modules and re-exports commonly used items for external use.

/// Noviq Programming Language
/// 
/// A simple, interpreted programming language written in Rust.
/// 
/// This library provides the core functionality for the Noviq language,
/// including lexing, parsing, and interpretation.

pub mod utils;
pub mod frontend;
pub mod runtime;

// Re-export commonly used items
pub use utils::{get_version, get_package_name, get_package_description};
pub use frontend::{Parser, Lexer};
pub use runtime::Interpreter;
