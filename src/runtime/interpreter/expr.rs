// runtime/interpreter/expr.rs
// Expression evaluation logic.
// Evaluates expression AST nodes into runtime values.

/// Expression evaluation

use crate::frontend::ast::Expr;
use crate::runtime::value::Value;
use crate::runtime::builtins;

pub struct ExprEvaluator;

impl ExprEvaluator {
    pub fn new() -> Self {
        ExprEvaluator
    }
    
    pub fn evaluate(&mut self, expr: Expr) -> Result<Value, String> {
        match expr {
            Expr::String(s) => Ok(Value::String(s)),
            Expr::Call { name, args } => {
                builtins::call_builtin(&name, args, &mut |e| self.evaluate(e))
            }
        }
    }
}

impl Default for ExprEvaluator {
    fn default() -> Self {
        Self::new()
    }
}
