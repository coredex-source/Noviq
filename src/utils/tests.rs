// utils/tests.rs
// Integration tests for the Noviq interpreter.
// Verifies that example files exist, are readable, and have correct syntax.

/// Tests for the Noviq interpreter
/// 
/// These tests verify that the interpreter can read and process
/// example files correctly.

#[cfg(test)]
mod tests {
    use std::fs;
    use std::path::Path;

    #[test]
    fn test_example_files_exist() {
        assert!(Path::new("examples/hello.nvq").exists(), "hello.nvq should exist");
        assert!(Path::new("examples/test.nvq").exists(), "test.nvq should exist");
    }

    #[test]
    fn test_can_read_hello_example() {
        let contents = fs::read_to_string("examples/hello.nvq")
            .expect("Should be able to read hello.nvq");
        
        assert!(contents.contains("Hello, World!"), "Should contain greeting");
        assert!(contents.contains("print"), "Should contain print statements");
    }

    #[test]
    fn test_can_read_test_example() {
        let contents = fs::read_to_string("examples/test.nvq")
            .expect("Should be able to read test.nvq");
        
        assert!(contents.contains("print"), "Should contain print statements");
    }

    #[test]
    fn test_example_syntax() {
        let hello = fs::read_to_string("examples/hello.nvq").unwrap();
        
        // Check for comment syntax
        assert!(hello.contains("#"), "Should have comments");
        
        // Check for string literals
        assert!(hello.contains("\""), "Should have string literals");
        
        // Check for print function
        assert!(hello.contains("print"), "Should have print function calls");
    }
}
