// frontend/token.rs
// Token type definitions for the lexer.
// Defines all token types that can be produced during tokenization.

use std::fmt;

/// Tokens produced by the lexer
#[derive(Debug, Clone, PartialEq)]
pub enum Token {
    // Keywords
    Let,
    
    // Identifiers and Literals
    Identifier(String),
    String(String),
    Number(f64),
    True,
    False,
    
    // Operators
    Assign,
    
    // Delimiters
    LeftParen,
    RightParen,
    Newline,
    Eof,
}

impl fmt::Display for Token {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Token::Let => write!(f, "let"),
            Token::Identifier(s) => write!(f, "Identifier({})", s),
            Token::String(s) => write!(f, "String(\"{}\")", s),
            Token::Number(n) => write!(f, "Number({})", n),
            Token::True => write!(f, "true"),
            Token::False => write!(f, "false"),
            Token::Assign => write!(f, "="),
            Token::LeftParen => write!(f, "("),
            Token::RightParen => write!(f, ")"),
            Token::Newline => write!(f, "Newline"),
            Token::Eof => write!(f, "EOF"),
        }
    }
}
