// frontend/ast.rs
// Abstract Syntax Tree (AST) node definitions.
// Defines the structure of parsed Noviq programs (expressions and statements).

/// AST node definitions for Noviq
///
/// Expressions and statements for the Noviq language

#[derive(Debug, Clone, PartialEq)]
pub enum Expr {
    String(String),
    Number(f64),
    Boolean(bool),
    Identifier(String),
    Call { name: String, args: Vec<Expr> },
}

#[derive(Debug, Clone, PartialEq)]
pub enum Stmt {
    Let { name: String, value: Expr },
    Expr(Expr),
}
