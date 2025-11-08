// frontend/lexer/reader.rs
// Character-level reading utilities for tokenization.
// Handles reading characters, strings, identifiers, and skipping whitespace/comments.

/// Character reading utilities for the lexer

pub struct Reader {
    input: Vec<char>,
    position: usize,
    pub current: Option<char>,
}

impl Reader {
    pub fn new(input: &str) -> Self {
        let chars: Vec<char> = input.chars().collect();
        let current = chars.get(0).copied();
        Reader {
            input: chars,
            position: 0,
            current,
        }
    }
    
    pub fn advance(&mut self) {
        self.position += 1;
        self.current = self.input.get(self.position).copied();
    }
    
    pub fn skip_whitespace(&mut self) {
        while let Some(ch) = self.current {
            if ch == ' ' || ch == '\t' || ch == '\r' {
                self.advance();
            } else {
                break;
            }
        }
    }
    
    pub fn skip_comment(&mut self) {
        if self.current == Some('#') {
            while self.current.is_some() && self.current != Some('\n') {
                self.advance();
            }
        }
    }
    
    pub fn read_string(&mut self) -> String {
        let quote_char = self.current.unwrap(); // Remember which quote (' or ")
        let mut result = String::new();
        self.advance(); // Skip opening quote
        
        while let Some(ch) = self.current {
            if ch == quote_char {
                self.advance(); // Skip closing quote
                break;
            } else if ch == '\\' {
                self.advance();
                if let Some(escaped) = self.current {
                    match escaped {
                        'n' => result.push('\n'),
                        't' => result.push('\t'),
                        'r' => result.push('\r'),
                        '"' => result.push('"'),
                        '\'' => result.push('\''),
                        '\\' => result.push('\\'),
                        _ => {
                            result.push('\\');
                            result.push(escaped);
                        }
                    }
                    self.advance();
                }
            } else {
                result.push(ch);
                self.advance();
            }
        }
        
        result
    }
    
    pub fn read_number(&mut self) -> f64 {
        let mut num_str = String::new();
        
        while let Some(ch) = self.current {
            if ch.is_numeric() || ch == '.' {
                num_str.push(ch);
                self.advance();
            } else {
                break;
            }
        }
        
        num_str.parse().unwrap_or(0.0)
    }
    
    pub fn read_identifier(&mut self) -> String {
        let mut ident = String::new();
        
        while let Some(ch) = self.current {
            if ch.is_alphanumeric() || ch == '_' {
                ident.push(ch);
                self.advance();
            } else {
                break;
            }
        }
        
        ident
    }
}
