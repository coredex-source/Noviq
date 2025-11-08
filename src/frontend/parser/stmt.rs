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
        let mut expr_parser = ExprParser::new(self.tokens, self.position);
        let expr = expr_parser.parse()?;
        
        // Skip optional newline after statement
        if self.current() == &Token::Newline {
            self.advance();
        }
        
        Ok(Stmt::Expr(expr))
    }
}
