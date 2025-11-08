// frontend/parser/expr.rs
// Expression parsing logic.
// Parses tokens into expression AST nodes (strings, function calls, etc.).

/// Expression parsing

use crate::frontend::token::Token;
use crate::frontend::ast::Expr;

pub struct ExprParser<'a> {
    tokens: &'a [Token],
    position: &'a mut usize,
}

impl<'a> ExprParser<'a> {
    pub fn new(tokens: &'a [Token], position: &'a mut usize) -> Self {
        ExprParser { tokens, position }
    }
    
    fn current(&self) -> &Token {
        self.tokens.get(*self.position).unwrap_or(&Token::Eof)
    }
    
    fn advance(&mut self) -> Token {
        let token = self.current().clone();
        *self.position += 1;
        token
    }
    
    fn expect(&mut self, expected: Token) -> Result<(), String> {
        if self.current() == &expected {
            self.advance();
            Ok(())
        } else {
            Err(format!("Expected {:?}, got {:?}", expected, self.current()))
        }
    }
    
    pub fn parse(&mut self) -> Result<Expr, String> {
        match self.current().clone() {
            Token::String(s) => {
                self.advance();
                Ok(Expr::String(s))
            }
            Token::Identifier(name) => {
                self.advance();
                
                // Must be a function call
                if self.current() == &Token::LeftParen {
                    self.advance(); // consume '('
                    let args = self.parse_arguments()?;
                    self.expect(Token::RightParen)?;
                    Ok(Expr::Call { name, args })
                } else {
                    Err(format!("Expected '(' after identifier '{}'", name))
                }
            }
            token => Err(format!("Unexpected token in expression: {:?}", token)),
        }
    }
    
    fn parse_arguments(&mut self) -> Result<Vec<Expr>, String> {
        let mut args = Vec::new();
        
        // Empty argument list
        if self.current() == &Token::RightParen {
            return Ok(args);
        }
        
        // Parse first argument
        let mut expr_parser = ExprParser::new(self.tokens, self.position);
        args.push(expr_parser.parse()?);
        
        // Parse remaining arguments (separated by commas in future)
        // For now, just single argument supported
        
        Ok(args)
    }
}
