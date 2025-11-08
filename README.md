```diff

@@ The language is currently in Alpha development stages. @@

@@ A lot of missing features and bugs are to be expected. @@
```

# Noviq

Noviq- The name Noviq is make up of two words: Nova(which means new) and unique.

- So, Nova + unique = Noviq.

**Noviq** = **Nova** (new) + **unique**- Noviq is just a simple interpreted programming language writtin in Rust which aims to be independent in the future, i.e, being writtin in Noviq itself similar to how rust does it.

A simple, interpreted programming language written in Rust with a focus on clarity and ease of use.
A language written to have an easy syntax and high performance.

Noviq aims to be a compiled language instead of an interpreted language in the future.

Builder tool for interpreter version is called: Photon or Photon-NVQ
The compiler will be called: Singularity or Singularity-NVQ

First release of Noviq is expected to be Pre-alpha 1.0.0.

## Implementation:
### Pre-aplha (Nebula):
- Basic syntax including but not limited to variables, print, conditionals, etc.

### Aplha (Protostar):
- Including but not limited to everything that includes a basic workstack for a language like loops, functions, libs, a lot more.
- Start building a simple compiler.

### Beta (Nova):
- More advanced features including OOP, etc.
- Compiler should be able to build most of the code.

### Release (Supernova):
- A polished interpreted release of everything implemented before it.
- A fully working compiler which will be polished in future builds.

## Building

### Using Photon (Recommended)

Photon is the official build tool for Noviq, providing a unified cross-platform build experience:

```bash
# Build Photon first
cd photon
cargo build --release

# Use Photon to build Noviq
./target/release/photon build              # Debug build
./target/release/photon build release      # Release build
./target/release/photon build snapshot     # Snapshot build with git hash

# Install binaries to libs/
./target/release/photon install

# Clean build artifacts
./target/release/photon clean
```

See [photon/README.md](photon/README.md) for more details.

### Using Build Scripts (Legacy)

```bash
./build.sh              # Debug build
./build.sh --release    # Release build
./build.sh --snapshot   # Optimized snapshot build
```

Windows: Use `build.ps1` or `build.bat` instead. Binaries are output to `libs/` folder.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

GPL-3.0 - See [LICENSE](LICENSE) for details.

## Version Scheme:

- **Pre-alpha (Nebula)**: `nebula-x.x.x`
- **Alpha (Protostar)**: `protostar-x.x.x`
- **Beta (Nova)**: `nova-x.x.x`
- **Release (Supernova)**: `supernova-x.x.x`
- **Snapshots (Pulsar)**: `type-x.x.x-pulsar.YYMMDD.GITHASH`

Current version: **nebula-0.0.0**
Snapshot/Github Actions Build version scheme: **nebula-0.0.1-pulsar.YYMMDD**

---

*Note: This project is in pre-alpha development. Features may change, and bugs are expected.*
