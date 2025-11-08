// frontend/lexer/mod.rs
// Main lexer implementation.
// Coordinates character reading and token generation from source code.

/// Lexer module
///
/// Tokenizes Noviq source code

mod reader;

use reader::Reader;
use crate::frontend::token::Token;

pub struct Lexer {
    reader: Reader,
}

impl Lexer {
    pub fn new(input: &str) -> Self {
        Lexer {
            reader: Reader::new(input),
        }
    }
    
    pub fn next_token(&mut self) -> Token {
        self.reader.skip_whitespace();
        
        // Handle comments
        if self.reader.current == Some('#') {
            self.reader.skip_comment();
            if self.reader.current == Some('\n') {
                self.reader.advance();
                return Token::Newline;
            } else if self.reader.current.is_none() {
                return Token::Eof;
            }
            return self.next_token();
        }
        
        match self.reader.current {
            None => Token::Eof,
            Some('\n') => {
                self.reader.advance();
                Token::Newline
            }
            Some('"') | Some('\'') => Token::String(self.reader.read_string()),
            Some('=') => {
                self.reader.advance();
                Token::Assign
            }
            Some(ch) if ch.is_alphabetic() || ch == '_' => {
                let ident = self.reader.read_identifier();
                // Check for keywords
                match ident.as_str() {
                    "let" => Token::Let,
                    "true" => Token::True,
                    "false" => Token::False,
                    _ => Token::Identifier(ident),
                }
            }
            Some(ch) if ch.is_numeric() => {
                Token::Number(self.reader.read_number())
            }
            Some('(') => {
                self.reader.advance();
                Token::LeftParen
            }
            Some(')') => {
                self.reader.advance();
                Token::RightParen
            }
            Some(_ch) => {
                // Unknown character, skip it
                self.reader.advance();
                self.next_token()
            }
        }
    }
    
    pub fn tokenize(&mut self) -> Vec<Token> {
        let mut tokens = Vec::new();
        
        loop {
            let token = self.next_token();
            if token == Token::Eof {
                tokens.push(token);
                break;
            }
            tokens.push(token);
        }
        
        tokens
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_tokenize_string() {
        let mut lexer = Lexer::new(r#""Hello, World!""#);
        let tokens = lexer.tokenize();
        assert_eq!(tokens[0], Token::String("Hello, World!".to_string()));
    }
    
    #[test]
    fn test_tokenize_identifier() {
        let mut lexer = Lexer::new("print");
        let tokens = lexer.tokenize();
        assert_eq!(tokens[0], Token::Identifier("print".to_string()));
    }
    
    #[test]
    fn test_tokenize_function_call() {
        let mut lexer = Lexer::new(r#"print("Hello")"#);
        let tokens = lexer.tokenize();
        assert_eq!(tokens[0], Token::Identifier("print".to_string()));
        assert_eq!(tokens[1], Token::LeftParen);
        assert_eq!(tokens[2], Token::String("Hello".to_string()));
        assert_eq!(tokens[3], Token::RightParen);
    }
}
