// frontend/mod.rs
// Frontend module declaration.
// Exports the lexer, parser, token types, and AST definitions.

/// Frontend - Lexer and Parser
/// 
/// This module handles tokenization and parsing of Noviq source code.

pub mod token;
pub mod ast;
pub mod lexer;
pub mod parser;

pub use token::Token;
pub use ast::{Expr, Stmt};
pub use lexer::Lexer;
pub use parser::Parser;
