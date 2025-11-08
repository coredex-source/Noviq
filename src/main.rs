// main.rs
// Entry point for the Noviq interpreter binary.
// Handles command-line arguments, reads source files, and invokes the parser/interpreter.

/// Noviq Programming Language - Main Entry Point
/// 
/// This is the initialization file for the Noviq interpreter.
/// The actual implementation is in the library modules.

use noviq::{get_version, get_package_description, Parser, Interpreter};
use std::env;
use std::fs;
use std::process;

fn main() {
    let args: Vec<String> = env::args().collect();
    
    // If no arguments, show version info
    if args.len() == 1 {
        println!("Noviq Programming Language");
        println!("Version: {}", get_version());
        println!();
        println!("{}", get_package_description());
        println!();
        println!("Usage: noviq <file.nvq>");
        println!("       noviq --version");
        println!("       noviq --help");
        return;
    }
    
    // Handle flags
    match args[1].as_str() {
        "--version" | "-v" => {
            println!("Noviq {}", get_version());
            return;
        }
        "--help" | "-h" => {
            print_help();
            return;
        }
        _ => {}
    }
    
    // Try to read and execute the file
    let filename = &args[1];
    
    // Check if file has .nvq extension
    if !filename.ends_with(".nvq") {
        eprintln!("Error: File must have .nvq extension");
        eprintln!("Usage: noviq <file.nvq>");
        process::exit(1);
    }
    
    match fs::read_to_string(filename) {
        Ok(contents) => {
            if let Err(e) = execute_program(&contents) {
                eprintln!("Error: {}", e);
                process::exit(1);
            }
        }
        Err(e) => {
            eprintln!("Error reading file '{}': {}", filename, e);
            process::exit(1);
        }
    }
}

fn print_help() {
    println!("Noviq Programming Language");
    println!("Version: {}", get_version());
    println!();
    println!("USAGE:");
    println!("    noviq <file.nvq>       Run a Noviq program");
    println!("    noviq --version       Show version information");
    println!("    noviq --help          Show this help message");
    println!();
    println!("EXAMPLES:");
    println!("    noviq examples/hello.nvq");
    println!("    noviq my_program.nvq");
}

fn execute_program(source: &str) -> Result<(), String> {
    // Parse the source code
    let mut parser = Parser::new(source);
    let statements = parser.parse()?;
    
    // Execute the statements
    let mut interpreter = Interpreter::new();
    interpreter.execute(statements)?;
    
    Ok(())
}
