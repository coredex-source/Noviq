// frontend/parser/mod.rs
// Main parser implementation.
// Orchestrates token-to-AST conversion, manages parsing state, and delegates to expr/stmt parsers.

/// Parser module
///
/// Converts tokens into an AST

mod expr;
mod stmt;

use stmt::StmtParser;
use crate::frontend::token::Token;
use crate::frontend::ast::Stmt;
use crate::frontend::lexer::Lexer;

pub struct Parser {
    tokens: Vec<Token>,
    position: usize,
}

impl Parser {
    pub fn new(source: &str) -> Self {
        let mut lexer = Lexer::new(source);
        let tokens = lexer.tokenize();
        Parser { tokens, position: 0 }
    }
    
    fn current(&self) -> &Token {
        self.tokens.get(self.position).unwrap_or(&Token::Eof)
    }
    
    fn advance(&mut self) {
        self.position += 1;
    }
    
    fn skip_newlines(&mut self) {
        while self.current() == &Token::Newline {
            self.advance();
        }
    }
    
    pub fn parse(&mut self) -> Result<Vec<Stmt>, String> {
        let mut statements = Vec::new();
        
        self.skip_newlines();
        
        while self.current() != &Token::Eof {
            let mut stmt_parser = StmtParser::new(&self.tokens, &mut self.position);
            let stmt = stmt_parser.parse()?;
            statements.push(stmt);
            self.skip_newlines();
        }
        
        Ok(statements)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::frontend::ast::Expr;
    
    #[test]
    fn test_parse_function_call() {
        let mut parser = Parser::new(r#"log("Hello")"#);
        let stmts = parser.parse().unwrap();
        assert_eq!(stmts.len(), 1);
        match &stmts[0] {
            Stmt::Expr(Expr::Call { name, args }) => {
                assert_eq!(name, "log");
                assert_eq!(args.len(), 1);
            }
            _ => panic!("Expected function call"),
        }
    }
}
