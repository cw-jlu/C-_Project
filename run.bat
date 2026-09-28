@echo off
rem Build with CMake and run (used by Code Runner and double-click)
cd /d "%~dp0"
cmake --preset debug >nul || exit /b 1
cmake --build --preset debug || exit /b 1
build\IMSystemXST.exe
