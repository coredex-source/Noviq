# Photon Quick Start

## First Time Setup

1. **Build Photon**:
   ```bash
   cd photon
   cargo build --release
   ```

2. **Check System Requirements**:
   ```bash
   ./target/release/photon check
   ```
   
   This verifies you have Rust, Cargo, Git, and other required tools installed.

3. **Add to PATH (Optional but recommended)**:
   ```bash
   # Linux/macOS - Add to ~/.bashrc or ~/.zshrc
   export PATH="$PATH:/path/to/Noviq/photon/target/release"
   
   # Or create a symlink
   sudo ln -s /path/to/Noviq/photon/target/release/photon /usr/local/bin/photon
   ```

## Common Workflows

### Development Workflow

```bash
# Quick debug build for testing
photon build

# Test your changes
../target/debug/noviq ../examples/hello.nvq
```

### Release Workflow

```bash
# Build optimized release
photon build release

# Install to libs/
photon install

# Verify
../libs/noviq-nebula-0.1.0-linux-x86_64 --version
```

### Snapshot Workflow (CI/CD)

```bash
# Build with git tracking
photon build snapshot

# Install with unique version
photon install

# Result: noviq-nebula-0.1.0-pulsar.251108.5d579ad-linux-x86_64
```

## Tips

- **Clean builds**: Run `photon clean` before major rebuilds
- **Check versions**: Built binaries show their version with `--version`
- **Multiple profiles**: You can have debug, release, and snapshot builds simultaneously
- **Cross-platform**: Same commands work on Linux, macOS, and Windows

## Troubleshooting

### "System requirements not met"
- Run `photon check` to see what's missing
- Install Rust: https://rustup.rs/
- Install Git: https://git-scm.com/downloads

### "cargo: command not found"
- Install Rust: https://rustup.rs/
- Make sure `~/.cargo/bin` is in your PATH

### "Failed to change to Noviq directory"
- Make sure you're running Photon from the `photon/` directory
- Or the parent directory should be the Noviq root

### Git hash shows as "dev" in snapshot builds
- Make sure you're in a git repository
- Commit your changes: `git add . && git commit -m "Your changes"`

## Advanced Usage

### Custom Build Flags

Photon uses standard Cargo under the hood. For custom builds:

```bash
# Direct cargo usage (from Noviq root)
SNAPSHOT=1 cargo build --profile=snapshot --features=your-feature
```

### Build Artifacts Location

- Debug: `target/debug/noviq`
- Release: `target/release/noviq`
- Snapshot: `target/snapshot/noviq`
- Installed: `libs/noviq-{version}-{os}-{arch}`

## Next Steps

- Read the full [Photon README](README.md)
- Explore [Noviq examples](../examples/)
- Check the [project structure](../STRUCTURE.md)
