// system_check.rs
// System requirements verification for Photon build tool

use std::process::Command;
use std::fmt;

/// Represents the result of a system check
#[derive(Debug, Clone)]
pub struct CheckResult {
    pub name: String,
    pub passed: bool,
    pub version: Option<String>,
    pub message: String,
}

impl CheckResult {
    fn new(name: &str, passed: bool, version: Option<String>, message: &str) -> Self {
        CheckResult {
            name: name.to_string(),
            passed,
            version,
            message: message.to_string(),
        }
    }
}

impl fmt::Display for CheckResult {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let status = if self.passed { "✓" } else { "✗" };
        let version_str = self.version.as_ref()
            .map(|v| format!(" ({})", v))
            .unwrap_or_default();
        
        write!(f, "[{}] {}{} - {}", status, self.name, version_str, self.message)
    }
}

/// Check if Rust toolchain is available and get version
pub fn check_rust() -> CheckResult {
    match Command::new("rustc").arg("--version").output() {
        Ok(output) if output.status.success() => {
            let version = String::from_utf8_lossy(&output.stdout);
            let version_str = version.trim().to_string();
            
            // Parse version number (format: "rustc 1.XX.X ...")
            let version_parts: Vec<&str> = version_str.split_whitespace().collect();
            let version_num = version_parts.get(1).unwrap_or(&"unknown");
            
            // Check if version is sufficient (1.70.0+)
            let is_sufficient = check_rust_version(version_num);
            
            if is_sufficient {
                CheckResult::new(
                    "Rust Compiler",
                    true,
                    Some(version_num.to_string()),
                    "Available"
                )
            } else {
                CheckResult::new(
                    "Rust Compiler",
                    false,
                    Some(version_num.to_string()),
                    "Version 1.70.0 or higher required"
                )
            }
        }
        _ => CheckResult::new(
            "Rust Compiler",
            false,
            None,
            "Not found - Install from https://rustup.rs"
        ),
    }
}

/// Check if Cargo is available and get version
pub fn check_cargo() -> CheckResult {
    match Command::new("cargo").arg("--version").output() {
        Ok(output) if output.status.success() => {
            let version = String::from_utf8_lossy(&output.stdout);
            let version_str = version.trim().to_string();
            
            // Parse version number (format: "cargo 1.XX.X ...")
            let version_parts: Vec<&str> = version_str.split_whitespace().collect();
            let version_num = version_parts.get(1).unwrap_or(&"unknown");
            
            CheckResult::new(
                "Cargo",
                true,
                Some(version_num.to_string()),
                "Available"
            )
        }
        _ => CheckResult::new(
            "Cargo",
            false,
            None,
            "Not found - Install with Rust from https://rustup.rs"
        ),
    }
}

/// Check if Git is available and get version
pub fn check_git() -> CheckResult {
    match Command::new("git").arg("--version").output() {
        Ok(output) if output.status.success() => {
            let version = String::from_utf8_lossy(&output.stdout);
            let version_str = version.trim().to_string();
            
            // Parse version number (format: "git version X.XX.X ...")
            let version_parts: Vec<&str> = version_str.split_whitespace().collect();
            let version_num = version_parts.get(2).unwrap_or(&"unknown");
            
            CheckResult::new(
                "Git",
                true,
                Some(version_num.to_string()),
                "Available"
            )
        }
        _ => CheckResult::new(
            "Git",
            false,
            None,
            "Not found - Install from https://git-scm.com"
        ),
    }
}

/// Check if LLVM is available (optional but recommended)
pub fn check_llvm() -> CheckResult {
    match Command::new("llvm-config").arg("--version").output() {
        Ok(output) if output.status.success() => {
            let version = String::from_utf8_lossy(&output.stdout);
            let version_str = version.trim().to_string();
            
            CheckResult::new(
                "LLVM",
                true,
                Some(version_str),
                "Available (optional)"
            )
        }
        _ => CheckResult::new(
            "LLVM",
            true, // Not critical, so we pass it
            None,
            "Not found (optional for future JIT support)"
        ),
    }
}

/// Parse and compare Rust version
fn check_rust_version(version_str: &str) -> bool {
    // Parse version like "1.70.0" or "1.70.0-beta.1"
    let parts: Vec<&str> = version_str.split('.').collect();
    if parts.len() < 2 {
        return false;
    }
    
    // Get major and minor version
    let major: u32 = parts[0].parse().unwrap_or(0);
    let minor_str = parts[1].split('-').next().unwrap_or("0");
    let minor: u32 = minor_str.parse().unwrap_or(0);
    
    // Check if >= 1.70.0
    major > 1 || (major == 1 && minor >= 70)
}

/// Run all system checks and return results
pub fn run_all_checks() -> Vec<CheckResult> {
    vec![
        check_rust(),
        check_cargo(),
        check_git(),
        check_llvm(),
    ]
}

/// Run all checks and print results, return whether all critical checks passed
pub fn verify_system_requirements(verbose: bool) -> bool {
    if verbose {
        println!("[*] Checking system requirements...");
        println!();
    }
    
    let results = run_all_checks();
    let mut all_critical_passed = true;
    
    for result in &results {
        if verbose {
            println!("    {}", result);
        }
        
        // Only Rust, Cargo, and Git are critical
        if !result.passed && result.name != "LLVM" {
            all_critical_passed = false;
        }
    }
    
    if verbose {
        println!();
    }
    
    if !all_critical_passed {
        eprintln!("[ERROR] System requirements not met!");
        eprintln!();
        eprintln!("Please install the missing requirements:");
        eprintln!();
        
        for result in &results {
            if !result.passed {
                eprintln!("  • {} - {}", result.name, result.message);
            }
        }
        
        eprintln!();
        eprintln!("Quick Install Guide:");
        eprintln!("  Rust & Cargo: https://rustup.rs");
        eprintln!("  Git:          https://git-scm.com/downloads");
        eprintln!();
    } else if verbose {
        println!("[✓] All system requirements satisfied!");
        println!();
    }
    
    all_critical_passed
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_rust_version_check() {
        assert!(check_rust_version("1.70.0"));
        assert!(check_rust_version("1.75.0"));
        assert!(check_rust_version("2.0.0"));
        assert!(!check_rust_version("1.69.0"));
        assert!(!check_rust_version("1.60.0"));
    }
}
