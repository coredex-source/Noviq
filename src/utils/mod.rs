// utils/mod.rs
// Utilities module declaration.
// Groups together version information, tests, and other helper functionality.

/// Utility modules
/// 
/// Contains version info, tests, and other utilities.

pub mod version;

#[cfg(test)]
pub mod tests;

pub use version::{get_version, get_package_name, get_package_description};
