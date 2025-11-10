// photon/src/main.rs
// Photon - Build tool for the Noviq programming language
// Compiles the Noviq interpreter with various build profiles

use std::env;
use std::path::Path;
use std::process::{Command, exit};
use std::fs;

mod system_check;

fn main() {
    print_banner();
    
    let args: Vec<String> = env::args().collect();
    
    // Parse command
    let command = if args.len() > 1 {
        args[1].as_str()
    } else {
        "build"
    };
    
    match command {
        "build" => {
            let profile = if args.len() > 2 { &args[2] } else { "debug" };
            build_noviq(profile);
        }
        "clean" => clean_build(),
        "install" => install_noviq(),
        "check" => check_system(),
        "help" | "--help" | "-h" => print_help(),
        "version" | "--version" | "-v" => print_version(),
        _ => {
            eprintln!("Unknown command: {}", command);
            eprintln!("Run 'photon help' for usage information");
            exit(1);
        }
    }
}

fn print_banner() {
    let version = env!("CARGO_PKG_VERSION");
    println!("╔═══════════════════════════════════════╗");
    println!("║           Photon Build Tool           ║");
    println!("║   Noviq Programming Language v{}   ║", version);
    println!("╚═══════════════════════════════════════╝");
    println!();
}

fn print_version() {
    let version = env!("CARGO_PKG_VERSION");
    let name = env!("CARGO_PKG_NAME");
    let description = env!("CARGO_PKG_DESCRIPTION");
    
    println!("{} v{}", name.chars().next().unwrap().to_uppercase().to_string() + &name[1..], version);
    println!("{}", description);
}

fn print_help() {
    println!("USAGE:");
    println!("    photon [COMMAND] [OPTIONS]");
    println!();
    println!("COMMANDS:");
    println!("    build [PROFILE]    Build Noviq interpreter");
    println!("                       Profiles: debug, release, snapshot");
    println!("                       Default: debug");
    println!();
    println!("    clean              Clean build artifacts");
    println!("    install            Install built binary to libs/");
    println!("    check              Check system requirements");
    println!("    help               Show this help message");
    println!("    version            Show version information");
    println!();
    println!("EXAMPLES:");
    println!("    photon build              # Debug build");
    println!("    photon build release      # Release build");
    println!("    photon build snapshot     # Snapshot build with git hash");
    println!("    photon clean              # Clean build directory");
    println!("    photon install            # Install to libs/");
    println!("    photon check              # Verify system requirements");
    println!();
    println!("BUILD PROFILES:");
    println!("    debug      - Unoptimized with debug symbols");
    println!("               - Version: nebula-X.Y.Z-pulsar.YYMMDD");
    println!();
    println!("    release    - Optimized, stripped binary");
    println!("               - Version: nebula-X.Y.Z");
    println!();
    println!("    snapshot   - Optimized with build number");
    println!("               - Version: nebula-X.Y.Z-pulsar.YYMMDD.GITHASH");
}

fn build_noviq(profile: &str) {
    // First, verify system requirements
    if !system_check::verify_system_requirements(false) {
        println!();
        system_check::verify_system_requirements(true);
        exit(1);
    }
    
    println!("[*] Building Noviq ({} mode)...", profile);
    println!();
    
    // Change to parent directory (where Noviq Cargo.toml is)
    let photon_dir = env::current_dir().expect("Failed to get current directory");
    let noviq_dir = photon_dir.parent().unwrap_or(&photon_dir);
    
    env::set_current_dir(noviq_dir).expect("Failed to change to Noviq directory");
    
    let mut cmd = Command::new("cargo");
    cmd.arg("build");
    
    match profile {
        "release" => {
            cmd.arg("--release");
        }
        "snapshot" => {
            cmd.env("SNAPSHOT", "1");
            cmd.arg("--profile=snapshot");
        }
        "debug" => {
            // Default, no extra args
        }
        _ => {
            eprintln!("[ERROR] Unknown profile: {}", profile);
            eprintln!("Valid profiles: debug, release, snapshot");
            exit(1);
        }
    }
    
    println!("Running: cargo build {}", 
        match profile {
            "release" => "--release",
            "snapshot" => "--profile=snapshot (SNAPSHOT=1)",
            _ => "",
        }
    );
    println!();
    
    let status = cmd.status().expect("Failed to execute cargo build");
    
    if !status.success() {
        eprintln!("[ERROR] Build failed!");
        exit(1);
    }
    
    println!();
    println!("[SUCCESS] Build successful!");
    
    // Get binary path and show version
    let binary_path = match profile {
        "release" => "target/release/noviq",
        "snapshot" => "target/snapshot/noviq",
        _ => "target/debug/noviq",
    };
    
    if Path::new(binary_path).exists() {
        println!();
        println!("Binary location: {}", binary_path);
        
        // Get version from binary
        if let Ok(output) = Command::new(binary_path).arg("--version").output() {
            let version = String::from_utf8_lossy(&output.stdout);
            println!("{}", version.trim());
        }
        
        println!();
        println!("[TIP] Run 'photon install' to copy to libs/ directory");
    }
}

fn clean_build() {
    println!("[*] Cleaning build artifacts...");
    
    // Change to parent directory
    let photon_dir = env::current_dir().expect("Failed to get current directory");
    let noviq_dir = photon_dir.parent().unwrap_or(&photon_dir);
    
    env::set_current_dir(noviq_dir).expect("Failed to change to Noviq directory");
    
    let status = Command::new("cargo")
        .arg("clean")
        .status()
        .expect("Failed to execute cargo clean");
    
    if status.success() {
        println!("[SUCCESS] Clean successful!");
    } else {
        eprintln!("[ERROR] Clean failed!");
        exit(1);
    }
}

fn install_noviq() {
    println!("[*] Installing Noviq binaries...");
    println!();
    
    // Change to parent directory
    let photon_dir = env::current_dir().expect("Failed to get current directory");
    let noviq_dir = photon_dir.parent().unwrap_or(&photon_dir);
    
    env::set_current_dir(noviq_dir).expect("Failed to change to Noviq directory");
    
    // Create libs directory
    fs::create_dir_all("libs").expect("Failed to create libs directory");
    
    // Detect OS
    let os = if cfg!(target_os = "windows") {
        "windows"
    } else if cfg!(target_os = "macos") {
        "darwin"
    } else if cfg!(target_os = "linux") {
        "linux"
    } else {
        "unknown"
    };
    
    let arch = if cfg!(target_arch = "x86_64") {
        "x86_64"
    } else if cfg!(target_arch = "aarch64") {
        "aarch64"
    } else {
        "unknown"
    };
    
    let ext = if cfg!(target_os = "windows") { ".exe" } else { "" };
    
    let profiles = vec![
        ("debug", "target/debug/noviq"),
        ("release", "target/release/noviq"),
        ("snapshot", "target/snapshot/noviq"),
    ];
    
    let mut installed = 0;
    
    for (profile, binary_path) in profiles {
        let full_path = format!("{}{}", binary_path, ext);
        
        if !Path::new(&full_path).exists() {
            println!("[SKIP] Skipping {} (not built)", profile);
            continue;
        }
        
        // Get version from binary
        let version = if let Ok(output) = Command::new(&full_path).arg("--version").output() {
            let v = String::from_utf8_lossy(&output.stdout);
            v.split_whitespace()
                .nth(1)
                .unwrap_or("unknown")
                .to_string()
        } else {
            "unknown".to_string()
        };
        
        let output_name = format!("libs/noviq-{}-{}-{}{}", version, os, arch, ext);
        
        match fs::copy(&full_path, &output_name) {
            Ok(_) => {
                println!("[OK] Installed: {} -> {}", profile, output_name);
                installed += 1;
            }
            Err(e) => {
                eprintln!("[ERROR] Failed to install {}: {}", profile, e);
            }
        }
    }
    
    println!();
    if installed > 0 {
        println!("[SUCCESS] Successfully installed {} binary(ies) to libs/", installed);
    } else {
        println!("[WARN] No binaries installed. Build Noviq first with 'photon build'");
    }
}

fn check_system() {
    println!("[*] Verifying system requirements...");
    println!();
    
    let results = system_check::run_all_checks();
    let mut all_critical_passed = true;
    
    // Print results
    for result in &results {
        println!("    {}", result);
        
        // Only Rust, Cargo, and Git are critical
        if !result.passed && result.name != "LLVM" {
            all_critical_passed = false;
        }
    }
    
    println!();
    
    if all_critical_passed {
        println!("[✓] All system requirements satisfied!");
        println!("    You're ready to build Noviq!");
    } else {
        println!("[✗] Some critical requirements are missing!");
        println!();
        println!("Please install the missing components:");
        
        for result in &results {
            if !result.passed && result.name != "LLVM" {
                println!("  • {} - {}", result.name, result.message);
            }
        }
        
        println!();
        println!("Quick Install Guide:");
        println!("  Rust & Cargo: https://rustup.rs");
        println!("  Git:          https://git-scm.com/downloads");
    }
}
