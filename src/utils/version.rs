// utils/version.rs
// Version string generation for Noviq.
// Provides functions to get version, package name, and description from Cargo.toml.

/// Version management for Noviq
/// 
/// Handles version string generation based on build type:
/// - Release builds: nebula-X.Y.Z
/// - Debug builds: nebula-X.Y.Z-pulsar.YYMMDD
/// - Snapshot builds: nebula-X.Y.Z-pulsar.YYMMDD.BUILD

/// Get the current version string for Noviq
pub fn get_version() -> String {
    // Get version from Cargo.toml
    let cargo_version = env!("CARGO_PKG_VERSION");
    let base_version = format!("nebula-{}", cargo_version);
    
    // Check if SNAPSHOT environment variable is set during build
    let is_snapshot = option_env!("SNAPSHOT").is_some();
    
    if is_snapshot {
        // Snapshot build - include date and build number
        let date = chrono::Local::now().format("%y%m%d").to_string();
        let build_number = get_build_number();
        format!("{}-pulsar.{}.{}", base_version, date, build_number)
    } else if cfg!(debug_assertions) {
        // Debug build - use snapshot format without build number
        let date = chrono::Local::now().format("%y%m%d").to_string();
        format!("{}-pulsar.{}", base_version, date)
    } else {
        // Release build - use base version
        base_version
    }
}

/// Get build number based on git commit hash
/// Returns the short git hash if available, otherwise "dev"
fn get_build_number() -> String {
    // Try to get git commit hash at compile time
    if let Some(git_hash) = option_env!("GIT_HASH") {
        git_hash.to_string()
    } else {
        // Fallback to "dev" if git hash not available
        "dev".to_string()
    }
}

/// Get package name
pub fn get_package_name() -> &'static str {
    env!("CARGO_PKG_NAME")
}

/// Get package description
pub fn get_package_description() -> &'static str {
    env!("CARGO_PKG_DESCRIPTION")
}
