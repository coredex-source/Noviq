// runtime/builtins/log.rs
// Implementation of the log() built-in function.
// Prints values to stdout, separated by spaces.

use crate::frontend::ast::Expr;
use crate::runtime::value::Value;

/// Implements the `log` builtin.
///
/// Evaluates each argument using the provided evaluator and prints them separated by spaces.
pub fn call<F>(args: Vec<Expr>, eval: &mut F) -> Result<Value, String>
where
    F: FnMut(Expr) -> Result<Value, String>,
{
    if args.is_empty() {
        println!();
        return Ok(Value::Null);
    }

    for (i, arg) in args.into_iter().enumerate() {
        let v = eval(arg)?;
        if i > 0 {
            print!(" ");
        }
        print!("{}", v);
    }
    println!();

    Ok(Value::Null)
}
