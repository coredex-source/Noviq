// frontend/ast.rs
// Abstract Syntax Tree (AST) node definitions.
// Defines the structure of parsed Noviq programs (expressions and statements).

/// AST node definitions for Noviq
///
/// Kept minimal for the current language subset (string literals and function calls)

#[derive(Debug, Clone, PartialEq)]
pub enum Expr {
    String(String),
    Call { name: String, args: Vec<Expr> },
}

#[derive(Debug, Clone, PartialEq)]
pub enum Stmt {
    Expr(Expr),
}
