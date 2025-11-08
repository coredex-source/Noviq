# Unified build script for Noviq (Windows)
param(
    [Parameter(Position=0)]
    [string]$BuildType
)

# Create libs directory if it doesn't exist
New-Item -ItemType Directory -Force -Path libs | Out-Null

function Build-Noviq {
    param($Type, $Profile, $EnvVars = @{})
    
    Write-Host "Building Noviq ($Type)..." -ForegroundColor Green
    
    # Set environment variables if any
    foreach ($key in $EnvVars.Keys) {
        $env:($key) = $EnvVars[$key]
    }
    
    # Build
    if ($Profile -eq "dev") {
        cargo build
        $BinaryPath = "target\debug\noviq.exe"
    } else {
        cargo build --profile $Profile
        $BinaryPath = "target\$Profile\noviq.exe"
    }
    
    # Get version
    $VersionOutput = & $BinaryPath
    $Version = ($VersionOutput | Select-String "Version:").ToString().Split()[1]
    
    # Determine architecture
    $Arch = if ([Environment]::Is64BitOperatingSystem) { "x86_64" } else { "x86" }
    $OutputName = "noviq-$Version-windows-$Arch.exe"
    
    # Copy to libs
    Copy-Item $BinaryPath "libs\$OutputName" -Force
    
    Write-Host ""
    Write-Host "$Type build complete!" -ForegroundColor Green
    Write-Host "Binary copied to: libs\$OutputName" -ForegroundColor Cyan
    Write-Host ""
    
    # Run the binary
    & $BinaryPath
}

switch ($BuildType) {
    "--release" {
        Build-Noviq -Type "Release" -Profile "release"
    }
    "--snapshot" {
        Build-Noviq -Type "Snapshot" -Profile "snapshot" -EnvVars @{SNAPSHOT="1"}
    }
    "" {
        Build-Noviq -Type "Debug" -Profile "dev"
    }
    default {
        Write-Host "Usage: .\build.ps1 [--release|--snapshot]"
        Write-Host ""
        Write-Host "Options:"
        Write-Host "  (no flag)   Build in debug mode with snapshot version"
        Write-Host "  --release   Build optimized release with clean version"
        Write-Host "  --snapshot  Build optimized snapshot with dated version"
        exit 1
    }
}
