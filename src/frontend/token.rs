// frontend/token.rs
// Token type definitions for the lexer.
// Defines all token types that can be produced during tokenization.

use std::fmt;

/// Tokens produced by the lexer
#[derive(Debug, Clone, PartialEq)]
pub enum Token {
    Identifier(String),
    String(String),
    LeftParen,
    RightParen,
    Newline,
    Eof,
}

impl fmt::Display for Token {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Token::Identifier(s) => write!(f, "Identifier({})", s),
            Token::String(s) => write!(f, "String(\"{}\")", s),
            Token::LeftParen => write!(f, "("),
            Token::RightParen => write!(f, ")"),
            Token::Newline => write!(f, "Newline"),
            Token::Eof => write!(f, "EOF"),
        }
    }
}
