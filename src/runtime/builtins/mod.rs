// runtime/builtins/mod.rs
// Built-in function router.
// Dispatches function calls by name to their respective implementations.

mod log;

use crate::frontend::ast::Expr;
use crate::runtime::value::Value;

pub use log::call as call_log;

/// Generic entry point for calling builtins by name.
///
/// `eval` is a mutable closure that evaluates an `Expr` into a `Value`.
pub fn call_builtin<F>(name: &str, args: Vec<Expr>, eval: &mut F) -> Result<Value, String>
where
    F: FnMut(Expr) -> Result<Value, String>,
{
    match name {
        "log" => call_log(args, eval),
        other => Err(format!("Unknown builtin: {}", other)),
    }
}
