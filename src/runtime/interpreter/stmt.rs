// runtime/interpreter/stmt.rs
// Statement execution logic.
// Executes statement AST nodes by delegating to expression evaluator.

/// Statement execution

use crate::frontend::ast::Stmt;
use super::expr::ExprEvaluator;

pub struct StmtExecutor;

impl StmtExecutor {
    pub fn execute(stmt: Stmt, evaluator: &mut ExprEvaluator) -> Result<(), String> {
        match stmt {
            Stmt::Let { name, value } => {
                let val = evaluator.evaluate(value)?;
                evaluator.set_variable(name, val);
                Ok(())
            }
            Stmt::Expr(expr) => {
                evaluator.evaluate(expr)?;
                Ok(())
            }
        }
    }
}
