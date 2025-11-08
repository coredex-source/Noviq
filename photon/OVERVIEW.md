# Photon - Noviq Build Tool

## Overview

**Photon** is the official build tool for the Noviq programming language. Written entirely in Rust, it provides a unified, cross-platform alternative to traditional shell and batch scripts for building the Noviq interpreter.

## Directory Structure

```
photon/
├── Cargo.toml           # Photon package configuration
├── README.md            # Full documentation
├── QUICKSTART.md        # Quick start guide
├── src/
│   └── main.rs          # Photon implementation
└── target/
    └── release/
        └── photon       # Built binary
```

## Features

**Cross-Platform** - Same commands work on Linux, macOS, Windows
**Multiple Build Profiles** - Debug, Release, Snapshot
**Automatic Installation** - Copies binaries to `libs/` with proper naming
**Git Integration** - Snapshot builds include commit hash
**Clean Interface** - Beautiful terminal output with emojis
**Self-Contained** - No external dependencies except Rust toolchain

## Commands

| Command | Description |
|---------|-------------|
| `photon build [profile]` | Build Noviq interpreter (debug/release/snapshot) |
| `photon clean` | Clean build artifacts |
| `photon install` | Install built binaries to libs/ |
| `photon help` | Show help message |
| `photon version` | Show Photon version |

## Build Profiles

### Debug (Default)
- Unoptimized, fast compilation
- Includes debug symbols
- Version: `nebula-X.Y.Z-pulsar.YYMMDD`
- Use for: Development and testing

### Release
- Fully optimized
- Stripped binary (smallest size)
- Version: `nebula-X.Y.Z`
- Use for: Production releases

### Snapshot
- Fully optimized
- Includes git commit hash
- Version: `nebula-X.Y.Z-pulsar.YYMMDD.GITHASH`
- Use for: CI/CD, testing builds with traceability

## Example Usage

```bash
# Initial setup
cd photon
cargo build --release

# Build Noviq in different modes
./target/release/photon build              # Debug
./target/release/photon build release      # Release
./target/release/photon build snapshot     # Snapshot

# Install all built binaries
./target/release/photon install

# Result in libs/:
# - noviq-nebula-0.1.0-pulsar.251108-linux-x86_64
# - noviq-nebula-0.1.0-linux-x86_64
# - noviq-nebula-0.1.0-pulsar.251108.5d579ad-linux-x86_64
```

## Binary Naming Convention

Installed binaries follow this pattern:
```
noviq-{VERSION}-{OS}-{ARCH}[.exe]
```

Examples:
- `noviq-nebula-0.1.0-linux-x86_64` (release)
- `noviq-nebula-0.1.0-pulsar.251108-darwin-aarch64` (debug)
- `noviq-nebula-0.1.0-pulsar.251108.5d579ad-windows-x86_64.exe` (snapshot)

## Advantages Over Shell Scripts

| Feature | Shell Scripts | Photon |
|---------|---------------|--------|
| Cross-platform | Need separate .sh, .bat, .ps1 | Single binary |
| Error handling | Limited | Robust Rust error handling |
| Version detection | Manual parsing | Automatic |
| Maintenance | Multiple scripts to update | Single codebase |
| User experience | Inconsistent output | Unified, beautiful output |

## Integration with Noviq

Photon is designed to work alongside Noviq:
- Reads Noviq's `Cargo.toml` for versioning
- Respects Noviq's build profiles (debug, release, snapshot)
- Outputs to Noviq's `libs/` directory
- Automatically detects and uses git information

## Requirements

- Rust toolchain (1.70+)
- Git (for snapshot builds)
- Parent directory must be the Noviq root

## Development

Photon itself is a simple Rust binary with minimal dependencies:
- `chrono` - For date formatting (same as Noviq)

To modify Photon:
1. Edit `src/main.rs`
2. Test with `cargo run -- [command]`
3. Build with `cargo build --release`

## Future Enhancements

Potential features for future versions:
- [ ] Parallel builds
- [ ] Custom profile support
- [ ] Build caching
- [ ] Remote installation
- [ ] Build statistics
- [ ] Watch mode (rebuild on file change)

## See Also

- [Photon README](README.md) - Full documentation
- [Photon QUICKSTART](QUICKSTART.md) - Quick start guide
- [Noviq STRUCTURE](../STRUCTURE.md) - Noviq project structure
- [Noviq README](../README.md) - Main Noviq documentation
