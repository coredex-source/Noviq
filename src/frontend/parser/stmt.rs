// frontend/parser/stmt.rs
// Statement parsing logic.
// Parses tokens into statement AST nodes by delegating to expression parser.

/// Statement parsing

use crate::frontend::token::Token;
use crate::frontend::ast::Stmt;
use super::expr::ExprParser;

pub struct StmtParser<'a> {
    tokens: &'a [Token],
    position: &'a mut usize,
}

impl<'a> StmtParser<'a> {
    pub fn new(tokens: &'a [Token], position: &'a mut usize) -> Self {
        StmtParser { tokens, position }
    }
    
    fn current(&self) -> &Token {
        self.tokens.get(*self.position).unwrap_or(&Token::Eof)
    }
    
    fn advance(&mut self) {
        *self.position += 1;
    }
    
    pub fn parse(&mut self) -> Result<Stmt, String> {
        // Check for let statement
        if self.current() == &Token::Let {
            return self.parse_let();
        }
        
        // Otherwise, parse as expression statement
        let mut expr_parser = ExprParser::new(self.tokens, self.position);
        let expr = expr_parser.parse()?;
        
        // Skip optional newline after statement
        if self.current() == &Token::Newline {
            self.advance();
        }
        
        Ok(Stmt::Expr(expr))
    }
    
    fn parse_let(&mut self) -> Result<Stmt, String> {
        self.advance(); // Skip 'let'
        
        // Get variable name
        let name = match self.current() {
            Token::Identifier(n) => n.clone(),
            _ => return Err("Expected identifier after 'let'".to_string()),
        };
        self.advance();
        
        // Expect '='
        if self.current() != &Token::Assign {
            return Err("Expected '=' after variable name".to_string());
        }
        self.advance();
        
        // Parse value expression
        let mut expr_parser = ExprParser::new(self.tokens, self.position);
        let value = expr_parser.parse()?;
        
        // Skip optional newline
        if self.current() == &Token::Newline {
            self.advance();
        }
        
        Ok(Stmt::Let { name, value })
    }
}
