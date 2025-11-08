// runtime/interpreter/expr.rs
// Expression evaluation logic.
// Evaluates expression AST nodes into runtime values.

/// Expression evaluation

use crate::frontend::ast::Expr;
use crate::runtime::value::Value;
use crate::runtime::builtins;
use std::collections::HashMap;

pub struct ExprEvaluator {
    variables: HashMap<String, Value>,
}

impl ExprEvaluator {
    pub fn new() -> Self {
        ExprEvaluator {
            variables: HashMap::new(),
        }
    }
    
    pub fn set_variable(&mut self, name: String, value: Value) {
        self.variables.insert(name, value);
    }
    
    pub fn evaluate(&mut self, expr: Expr) -> Result<Value, String> {
        match expr {
            Expr::String(s) => Ok(Value::String(s)),
            Expr::Number(n) => Ok(Value::Number(n)),
            Expr::Boolean(b) => Ok(Value::Boolean(b)),
            Expr::Identifier(name) => {
                self.variables
                    .get(&name)
                    .cloned()
                    .ok_or_else(|| format!("Undefined variable: {}", name))
            }
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
