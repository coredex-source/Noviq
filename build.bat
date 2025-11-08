@echo off
REM Unified build script for Noviq (Windows CMD)

if not exist libs mkdir libs

if "%1"=="--release" goto release
if "%1"=="--snapshot" goto snapshot
if "%1"=="" goto debug
goto usage

:debug
echo Building Noviq (Debug)...
cargo build
if errorlevel 1 exit /b 1

for /f "tokens=2" %%i in ('target\debug\noviq.exe ^| findstr "Version:"') do set VERSION=%%i
set OUTPUT_NAME=noviq-%VERSION%-windows-x86_64.exe
copy target\debug\noviq.exe libs\%OUTPUT_NAME% >nul

echo.
echo Debug build complete!
echo Binary copied to: libs\%OUTPUT_NAME%
echo.
target\debug\noviq.exe
goto end

:release
echo Building Noviq (Release)...
cargo build --release
if errorlevel 1 exit /b 1

for /f "tokens=2" %%i in ('target\release\noviq.exe ^| findstr "Version:"') do set VERSION=%%i
set OUTPUT_NAME=noviq-%VERSION%-windows-x86_64.exe
copy target\release\noviq.exe libs\%OUTPUT_NAME% >nul

echo.
echo Release build complete!
echo Binary copied to: libs\%OUTPUT_NAME%
echo.
target\release\noviq.exe
goto end

:snapshot
echo Building Noviq (Snapshot)...
set SNAPSHOT=1
cargo build --profile snapshot
if errorlevel 1 exit /b 1

for /f "tokens=2" %%i in ('target\snapshot\noviq.exe ^| findstr "Version:"') do set VERSION=%%i
set OUTPUT_NAME=noviq-%VERSION%-windows-x86_64.exe
copy target\snapshot\noviq.exe libs\%OUTPUT_NAME% >nul

echo.
echo Snapshot build complete!
echo Binary copied to: libs\%OUTPUT_NAME%
echo.
target\snapshot\noviq.exe
goto end

:usage
echo Usage: build.bat [--release^|--snapshot]
echo.
echo Options:
echo   (no flag)   Build in debug mode with snapshot version
echo   --release   Build optimized release with clean version
echo   --snapshot  Build optimized snapshot with dated version
exit /b 1

:end
