// runtime/interpreter/mod.rs
// Main interpreter implementation.
// Orchestrates AST execution by coordinating statement and expression evaluation.

/// Interpreter module
///
/// Executes Noviq AST

mod stmt;
mod expr;

use stmt::StmtExecutor;
use expr::ExprEvaluator;
use crate::frontend::ast::Stmt;

pub struct Interpreter {
    evaluator: ExprEvaluator,
}

impl Interpreter {
    pub fn new() -> Self {
        Interpreter {
            evaluator: ExprEvaluator::new(),
        }
    }
    
    pub fn execute(&mut self, statements: Vec<Stmt>) -> Result<(), String> {
        for stmt in statements {
            StmtExecutor::execute(stmt, &mut self.evaluator)?;
        }
        Ok(())
    }
}

impl Default for Interpreter {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::frontend::Parser;
    
    #[test]
    fn test_execute_function_call() {
        let mut parser = Parser::new(r#"print("Hello")"#);
        let stmts = parser.parse().unwrap();
        let mut interpreter = Interpreter::new();
        assert!(interpreter.execute(stmts).is_ok());
    }
}
