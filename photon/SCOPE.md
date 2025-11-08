# Photon - Future Scope & Roadmap

## Vision

Transform Photon from a simple build script replacement into a comprehensive development toolchain for Noviq, similar to how Cargo serves Rust or npm serves Node.js.

## Current State (v0.1.0)

- Cross-platform build tool
- Multiple build profiles (debug, release, snapshot)
- Binary installation to libs/
- Git integration for version tracking
- Clean build artifacts

## Planned Features

### Phase 1: Enhanced Build System (v0.2.0)

#### Dependency Management
- **System Requirements Check**
  - Verify Rust toolchain version
  - Check Git availability
  - Validate required system libraries
  - Display helpful error messages with installation links

- **Build Caching**
  - Track source file changes
  - Skip unnecessary rebuilds
  - Cache intermediate artifacts
  - Show build time improvements

- **Build Statistics**
  - Compilation time tracking
  - Binary size comparisons
  - Historical build data
  - Performance regression detection

#### Developer Experience
- **Progress Indicators**
  - Real-time build progress bars
  - Estimated time remaining
  - Current compilation unit display

- **Better Error Messages**
  - Parse cargo output for friendly errors
  - Suggest fixes for common issues
  - Link to documentation

### Phase 2: Development Workflow (v0.3.0)

#### Watch Mode
```bash
photon watch [file]      # Auto-rebuild on changes
photon watch --run       # Auto-rebuild and execute
photon watch --test      # Auto-run tests on changes
```

Features:
- File system monitoring
- Debounced rebuilds
- Clear screen between builds
- Sound/notification on success/failure

#### Testing Integration
```bash
photon test              # Run all tests
photon test [name]       # Run specific test
photon test --watch      # Watch mode for tests
photon test --coverage   # Generate coverage report
```

Features:
- Unified test runner
- Pretty test output formatting
- Test result caching
- Parallel test execution

#### Benchmarking
```bash
photon bench             # Run benchmarks
photon bench --compare   # Compare with previous run
photon bench --save      # Save baseline
```

### Phase 3: Code Quality Tools (v0.4.0)

#### Static Analysis
```bash
photon check             # Quick syntax check
photon lint              # Run linter
photon format            # Format code
photon format --check    # Check formatting
```

Features:
- Fast syntax validation
- Style guide enforcement
- Auto-fix common issues
- IDE-friendly output format

#### Documentation
```bash
photon doc               # Generate documentation
photon doc --open        # Generate and open in browser
photon doc --serve       # Documentation server
```

### Phase 4: Project Management (v0.5.0)

#### Project Scaffolding
```bash
photon new [name]        # Create new project
photon init              # Initialize in existing directory
photon add builtin [name] # Add builtin function template
photon add example [name] # Add example file
```

Project templates:
- Minimal (just main.nvq)
- Standard (with examples and tests)
- Library (for reusable code)

#### Configuration System
```toml
# photon.toml
[project]
name = "my-noviq-app"
version = "0.1.0"

[build]
default-profile = "debug"
target-dir = "target"
output-dir = "libs"

[profiles.custom]
opt-level = 2
strip = false

[dependencies]
# Future: Noviq package dependencies

[scripts]
pre-build = "echo Building..."
post-build = "echo Done!"
```

### Phase 5: Advanced Features (v0.6.0+)

#### Multi-Target Builds
```bash
photon build --target windows     # Cross-compile
photon build --all-targets        # Build for all platforms
photon build --arch aarch64       # Specific architecture
```

#### Interactive Mode
```bash
photon interactive               # Launch interactive menu
```

Features:
- Select build profile with arrow keys
- View build history
- Manage installed binaries
- Configuration wizard

#### REPL Integration
```bash
photon repl                      # Start Noviq REPL
photon repl --load file.nvq      # Load file into REPL
```

#### Profiling & Debugging
```bash
photon profile app.nvq           # Performance profiling
photon trace app.nvq             # Execution tracing
photon debug app.nvq             # Launch debugger
```

### Phase 6: Package Management (v1.0.0)

#### Package System
```bash
photon package                   # Create package
photon publish                   # Publish to registry
photon install [package]         # Install package
photon update                    # Update packages
photon search [query]            # Search packages
```

Features:
- Semantic versioning
- Dependency resolution
- Lock files
- Private registries support

#### Build Plugins
```bash
photon plugin install [name]     # Install plugin
photon plugin list               # List plugins
```

Plugin capabilities:
- Custom build steps
- Code generators
- Linters and formatters
- Deployment tools

### Phase 7: CI/CD & Distribution (v1.1.0+)

#### CI/CD Integration
```bash
photon ci init github            # Generate GitHub Actions
photon ci init gitlab            # Generate GitLab CI
photon release                   # Automated releases
```

#### Distribution
```bash
photon docker                    # Build Docker image
photon package deb               # Create .deb package
photon package rpm               # Create .rpm package
photon package msi               # Create Windows installer
```

## Technical Implementation Notes

### Dependencies to Add
- `notify` - File system watching
- `indicatif` - Progress bars
- `colored` - Terminal colors
- `clap` - Better CLI parsing
- `serde` - Configuration parsing
- `toml` - Config file format
- `reqwest` - HTTP for package registry
- `tar` / `zip` - Package compression

### Architecture Improvements
- Plugin system architecture
- Configuration management layer
- Build cache database
- Task scheduling system
- Event system for hooks

### Performance Considerations
- Parallel compilation when possible
- Efficient file watching
- Minimal rebuild detection
- Cached dependency resolution

## Priority Ranking

### High Priority (Should implement soon)
1. **Watch mode** - Dramatically improves dev experience
2. **Testing integration** - Essential for quality
3. **Dependency checking** - Better user experience
4. **Progress indicators** - Professional feel

### Medium Priority (Nice to have)
5. **Build caching** - Performance improvement
6. **Configuration system** - Flexibility
7. **Project scaffolding** - Easier onboarding
8. **Static analysis tools** - Code quality

### Low Priority (Future enhancements)
9. **Package management** - When ecosystem grows
10. **Multi-target builds** - When needed
11. **Profiling tools** - For optimization
12. **CI/CD generators** - Automation

## Success Metrics

- **Adoption**: % of Noviq developers using Photon
- **Build Time**: Average reduction in build times
- **Developer Satisfaction**: Survey results
- **Feature Usage**: Which features are most used
- **Bug Reports**: Decreasing over time

## Community Involvement

Potential areas for community contributions:
- Plugin development
- Documentation improvements
- Platform-specific testing
- Feature requests and feedback
- Build profile templates

## Version Timeline (Estimated)

- **v0.2.0** - Q1 2026: Enhanced build system
- **v0.3.0** - Q2 2026: Watch mode & testing
- **v0.4.0** - Q3 2026: Code quality tools
- **v0.5.0** - Q4 2026: Project management
- **v0.6.0** - Q1 2027: Advanced features
- **v1.0.0** - Q2 2027: Package management

## References & Inspiration

Learning from existing tools:
- **Cargo** (Rust) - Build system, package management
- **npm/yarn** (Node.js) - Package ecosystem
- **Maven/Gradle** (Java) - Build lifecycle
- **Make** (C/C++) - Task automation
- **deno** (TypeScript) - Single binary toolchain
- **go** (Go) - Simplicity and speed

## Notes

- This is a living document and will evolve
- Features may be added, removed, or reprioritized
- Community feedback is essential
- Implementation should remain optional (don't break simple use cases)
- Keep Photon fast and lightweight

---

*Last Updated: November 8, 2025*
*Current Version: v0.1.0*
*Status: Foundation Complete*
