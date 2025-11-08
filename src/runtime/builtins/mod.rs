// runtime/builtins/mod.rs
// Built-in function router.
// Dispatches function calls by name to their respective implementations.

mod print;

use crate::frontend::ast::Expr;
use crate::runtime::value::Value;

pub use print::call as call_print;

/// Generic entry point for calling builtins by name.
///
/// `eval` is a mutable closure that evaluates an `Expr` into a `Value`.
pub fn call_builtin<F>(name: &str, args: Vec<Expr>, eval: &mut F) -> Result<Value, String>
where
    F: FnMut(Expr) -> Result<Value, String>,
{
    match name {
        "print" => call_print(args, eval),
        other => Err(format!("Unknown builtin: {}", other)),
    }
}
