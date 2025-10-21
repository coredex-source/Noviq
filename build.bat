@echo off
REM Build script for Noviq on Windows
REM Requires GCC (MinGW or similar) to be installed and in PATH

echo Building Noviq...

gcc -o noviq.exe -Iinclude ^
    src\main.c ^
    src\interpreter\interpreter.c ^
    src\interpreter\variables.c ^
    src\interpreter\expressions.c ^
    src\operators\arithmetic.c ^
    src\operators\logical.c ^
    src\operators\comparison.c ^
    src\statements\display.c ^
    src\statements\input.c ^
    src\statements\control_flow.c ^
    src\utils\error.c ^
    -lm

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful! Executable: noviq.exe
    echo.
    echo Usage:
    echo   noviq.exe -e ^<file^>    Execute a Noviq script
    echo   noviq.exe -h          Show help
    echo.
) else (
    echo.
    echo Build failed with error code %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)
