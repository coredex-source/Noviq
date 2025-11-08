# Photon

**The official build tool for Noviq Programming Language**

Photon is a cross-platform build tool written in Rust that compiles the Noviq interpreter. It replaces the traditional shell and batch scripts with a unified, platform-independent solution.

## Features

- Fast & Reliable - Built in Rust for performance and safety
- Multiple Build Profiles - Debug, Release, and Snapshot builds
- Automatic Installation - Copy built binaries to `libs/` with proper naming
- Clean Builds - Easy artifact cleanup
- Beautiful Output - Clear, colorful feedback during builds
- Cross-Platform - Works on Linux, macOS, and Windows

## Installation

Build Photon from the Noviq repository:

```bash
cd photon
cargo build --release
```

The binary will be at `photon/target/release/photon` (or `photon.exe` on Windows).

For convenience, you can add it to your PATH or create an alias:

```bash
# Linux/macOS
alias photon='/path/to/Noviq/photon/target/release/photon'

# Or copy to a directory in PATH
sudo cp target/release/photon /usr/local/bin/
```

## Usage

### Build Commands

```bash
# Build in debug mode (default)
photon build

# Build in release mode
photon build release

# Build snapshot with git commit hash
photon build snapshot
```

### Other Commands

```bash
# Clean build artifacts
photon clean

# Install built binaries to libs/
photon install

# Show help
photon help

# Show version
photon version
```

## Build Profiles

### Debug
- **Unoptimized** with debug symbols
- Fast compilation
- Larger binary size
- Version format: `nebula-X.Y.Z-pulsar.YYMMDD`

```bash
photon build debug
# or simply
photon build
```

### Release
- **Fully optimized** production build
- Stripped binary (smaller size)
- Clean version number
- Version format: `nebula-X.Y.Z`

```bash
photon build release
```

### Snapshot
- **Optimized** build with tracking
- Includes git commit hash for traceability
- Perfect for testing and CI/CD
- Version format: `nebula-X.Y.Z-pulsar.YYMMDD.GITHASH`

```bash
photon build snapshot
```

## Workflow Example

```bash
# 1. Make changes to Noviq source code
cd /path/to/Noviq

# 2. Build with Photon
cd photon
./target/release/photon build snapshot

# 3. Install to libs/
./target/release/photon install

# 4. Test the build
cd ..
./libs/noviq-nebula-0.1.0-pulsar.251108.f60f108-linux-x86_64 examples/hello.nvq
```

## Binary Naming Convention

Installed binaries follow this naming pattern:

```
noviq-{VERSION}-{OS}-{ARCH}[.exe]
```

Examples:
- `noviq-nebula-0.1.0-linux-x86_64`
- `noviq-nebula-0.1.0-pulsar.251108-darwin-aarch64`
- `noviq-nebula-0.1.0-pulsar.251108.f60f108-windows-x86_64.exe`

## Requirements

- Rust toolchain (for building Photon)
- Git (for snapshot builds with commit hash)

## Integration with Cargo Workspace

Photon is designed to work alongside Noviq in the same repository but maintains its own build system. This keeps the build tool independent from the language implementation.

## Why Photon?

Traditional build scripts (`.sh`, `.bat`, `.ps1`) have several limitations:
- Platform-specific syntax
- Difficult to maintain across OSes
- Limited error handling
- Inconsistent behavior

Photon solves these issues by providing a single, unified build tool that works the same way on all platforms.

## License

Photon is part of the Noviq project and shares the same license.
