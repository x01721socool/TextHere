@echo off
setlocal enabledelayedexpansion

REM Create build directory
if not exist build mkdir build

REM Compile
gcc -Wall -Wextra -std=c99 ^
    -IC:/raylib/include -I./src ^
    -o build/game.exe ^
    src/game.c src/core/dialogue.c src/core/grid.c src/core/player.c ^
    -LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm -static

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b 1
)

echo Build successful!
if "%1"=="run" (
    build\game.exe
)