#!/usr/bin/env bash
# Unified build script for Noviq

set -e

# Create libs directory if it doesn't exist
mkdir -p libs

# Detect OS and set extension
EXT=""
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    EXT=".exe"
fi

case "$1" in
    --release)
        echo "Building Noviq (Release)..."
        cargo build --release
        
        # Get version for filename
        VERSION=$(./target/release/noviq${EXT} | grep "Version:" | awk '{print $2}')
        OUTPUT_NAME="noviq-${VERSION}-$(uname -s | tr '[:upper:]' '[:lower:]')-$(uname -m)${EXT}"
        
        # Copy to libs
        cp "target/release/noviq${EXT}" "libs/${OUTPUT_NAME}"
        
        echo ""
        echo "Release build complete!"
        echo "Binary copied to: libs/${OUTPUT_NAME}"
        echo ""
        ./target/release/noviq${EXT}
        ;;
    --snapshot)
        echo "Building Noviq (Snapshot)..."
        env SNAPSHOT=1 cargo build --profile snapshot
        
        # Get version for filename
        VERSION=$(./target/snapshot/noviq${EXT} | grep "Version:" | awk '{print $2}')
        OUTPUT_NAME="noviq-${VERSION}-$(uname -s | tr '[:upper:]' '[:lower:]')-$(uname -m)${EXT}"
        
        # Copy to libs
        cp "target/snapshot/noviq${EXT}" "libs/${OUTPUT_NAME}"
        
        echo ""
        echo "Snapshot build complete!"
        echo "Binary copied to: libs/${OUTPUT_NAME}"
        echo ""
        ./target/snapshot/noviq${EXT}
        ;;
    "")
        echo "Building Noviq (Debug)..."
        cargo build
        
        # Get version for filename
        VERSION=$(./target/debug/noviq${EXT} | grep "Version:" | awk '{print $2}')
        OUTPUT_NAME="noviq-${VERSION}-$(uname -s | tr '[:upper:]' '[:lower:]')-$(uname -m)${EXT}"
        
        # Copy to libs
        cp "target/debug/noviq${EXT}" "libs/${OUTPUT_NAME}"
        
        echo ""
        echo "Debug build complete!"
        echo "Binary copied to: libs/${OUTPUT_NAME}"
        echo ""
        ./target/debug/noviq${EXT}
        ;;
    *)
        echo "Usage: ./build.sh [--release|--snapshot]"
        echo ""
        echo "Options:"
        echo "  (no flag)   Build in debug mode with snapshot version"
        echo "  --release   Build optimized release with clean version"
        echo "  --snapshot  Build optimized snapshot with dated version"
        exit 1
        ;;
esac
