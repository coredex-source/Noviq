# Photon Changelog

All notable changes to this project will be documented in this file.

## [0.1.1] - 2025-11-10

### Added
- **System Requirements Check** feature from Phase 1 roadmap
  - New `photon check` command to verify system requirements
  - Automatic verification before each build
  - Checks for Rust compiler (1.70.0+), Cargo, Git, and LLVM
  - Helpful error messages with installation links when requirements are missing
  - Version detection for all required tools
  - Distinguishes between critical requirements (Rust, Cargo, Git) and optional (LLVM)
  - Unit tests for version checking logic

### Changed
- Updated help documentation to include `photon check` command
- Enhanced build process to verify system requirements before compilation
- Updated README.md with system requirements section
- Updated QUICKSTART.md with system check step
- Updated SCOPE.md to mark System Requirements Check as complete

### Technical Details
- New module: `src/system_check.rs`
- Dependencies: Uses standard library only (no new external dependencies)
- Cross-platform compatible (Linux, macOS, Windows)

## [0.1.0] - 2025-11-08

### Added
- Initial release of Photon build tool
- Cross-platform build support (Linux, macOS, Windows)
- Multiple build profiles: debug, release, snapshot
- Binary installation to libs/ with platform-specific naming
- Git integration for version tracking
- Clean build artifacts command
- Beautiful CLI output with ASCII banner
- Comprehensive help system
- Version information command

### Features
- `photon build [profile]` - Build Noviq interpreter
- `photon clean` - Clean build artifacts
- `photon install` - Install binaries to libs/
- `photon help` - Show help information
- `photon version` - Show version information

---

## Version Format

Photon follows [Semantic Versioning](https://semver.org/):
- MAJOR.MINOR.PATCH (e.g., 0.1.1)
- MAJOR: Breaking changes
- MINOR: New features (backward compatible)
- PATCH: Bug fixes (backward compatible)
