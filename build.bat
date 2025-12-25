@echo off
setlocal EnableDelayedExpansion

REM ================================
REM Detectar modo
REM ================================
set BUILD_TYPE=Release
if /I "%~1"=="debug" (
    echo [INFO] Debug mode enabled
    set BUILD_TYPE=Debug
)

if /I "%~1"=="rebuild" (
    echo [INFO] Rebuild mode enabled
    IF EXIST build (
        rd /s /q build
    )
)

REM ================================
REM Verificar Ninja
REM ================================
where ninja >nul 2>nul
if errorlevel 1 (
    echo [INFO] Ninja not found. Installing...
    winget install Ninja-build.Ninja -e --source winget
    if errorlevel 1 exit /b 1
)

REM ================================
REM Configurar y Compilar
REM ================================
echo [INFO] Configuring and Building (%BUILD_TYPE%)...

:: Usamos -G Ninja para asegurar consistencia
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if errorlevel 1 exit /b 1

cmake --build build
if errorlevel 1 exit /b 1

REM ================================
REM Ejecutar y Limpiar (Solo en Debug)
REM ================================
if "%BUILD_TYPE%"=="Release" (
    echo [INFO] Running application...
    
    if exist openGL.exe (
        .\openGL.exe
        
        echo [INFO] Closing program, starting cleanup...
        del /f openGL.exe
        echo [SUCCESS] Executable removed.
    ) else (
        echo [ERROR] openGL.exe not found in root.
    )
)
endlocal