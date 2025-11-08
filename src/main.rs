fn get_version() -> String {
    // Get version from Cargo.toml
    let cargo_version = env!("CARGO_PKG_VERSION");
    let base_version = format!("nebula-{}", cargo_version);
    
    // Check if SNAPSHOT environment variable is set during build
    let is_snapshot = option_env!("SNAPSHOT").is_some();
    
    if is_snapshot || cfg!(debug_assertions) {
        // Snapshot or Debug build - use snapshot format
        let date = chrono::Local::now().format("%y%m%d").to_string();
        format!("{}-pulsar.{}", base_version, date)
    } else {
        // Release build - use base version
        base_version
    }
}

fn main() {
    println!("Noviq Programming Language");
    println!("Version: {}", get_version());
    println!();
    println!("A simple, compiled programming language written in Rust.");
}
