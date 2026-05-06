@echo off
cd /d "%~dp0"

if not exist "backend\enigma_backend.exe" (
    echo Error: Backend executable not found! Please run 'mingw32-make' first.
    pause
    exit /b
)

if exist "enigma_env\Scripts\pythonw.exe" (
    start "" "enigma_env\Scripts\pythonw.exe" "frontend\main.py"
) else (
    start "" "pythonw.exe" "frontend\main.py"
)