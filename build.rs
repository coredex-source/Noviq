// build.rs
// Cargo build script to capture git information at compile time

use std::process::Command;

fn main() {
    // Get git commit hash (short version)
    if let Ok(output) = Command::new("git")
        .args(&["rev-parse", "--short", "HEAD"])
        .output()
    {
        if output.status.success() {
            let git_hash = String::from_utf8_lossy(&output.stdout);
            let git_hash = git_hash.trim();
            println!("cargo:rustc-env=GIT_HASH={}", git_hash);
        }
    }
    
    // Rerun if git HEAD changes (commits made)
    println!("cargo:rerun-if-changed=.git/HEAD");
    println!("cargo:rerun-if-changed=.git/refs/heads");
}
