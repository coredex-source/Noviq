// utils/version.rs
// Version string generation for Noviq.
// Provides functions to get version, package name, and description from Cargo.toml.

/// Version management for Noviq
/// 
/// Handles version string generation based on build type:
/// - Release builds: nebula-X.Y.Z
/// - Debug/Snapshot builds: nebula-X.Y.Z-pulsar.YYMMDD

/// Get the current version string for Noviq
pub fn get_version() -> String {
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

/// Get package name
pub fn get_package_name() -> &'static str {
    env!("CARGO_PKG_NAME")
}

/// Get package description
pub fn get_package_description() -> &'static str {
    env!("CARGO_PKG_DESCRIPTION")
}
