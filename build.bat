@echo off
setlocal EnableDelayedExpansion

REM ================================
REM Configuración general
REM ================================
set BUILD_TYPE=Release
set MSYS2_ROOT=C:\msys64
set BASH=%MSYS2_ROOT%\usr\bin\bash.exe
set GDB_PATH=%MSYS2_ROOT%\mingw64\bin\gdb.exe

REM ================================
REM Detectar modo
REM ================================
if /I "%~1"=="debug" (
    echo [BUILD INFO] Debug mode enabled
    set BUILD_TYPE=Debug
    call :check_gdb
)

if /I "%~1"=="rebuild" (
    echo [BUILD INFO] Rebuild mode enabled
    if exist build rd /s /q build
)

REM ================================
REM Verificar Ninja
REM ================================
where ninja >nul 2>nul || (
    echo [BUILD INFO] Ninja not found. Installing...
    winget install Ninja-build.Ninja -e --source winget || exit /b 1
)

REM ================================
REM Configurar y compilar
REM ================================
echo [BUILD INFO] Configuring and Building (%BUILD_TYPE%)...

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=%BUILD_TYPE% || exit /b 1
cmake --build build || exit /b 1

REM ================================
REM Ejecutar y limpiar (Release)
REM ================================
if "%BUILD_TYPE%"=="Release" (
    if exist openGL.exe (
        echo [BUILD INFO] Executing...
        .\openGL.exe
        del /f openGL.exe
        echo [BUILD SUCCESS] Executable removed.
    ) else (
        echo [BUILD ERROR] .exe not found.
    )
)

endlocal
exit /b 0

REM ================================
REM Funciones
REM ================================
:check_gdb
if not exist "%BASH%" (
    echo [ERROR] MSYS2 not found in %MSYS2_ROOT%
    exit /b 1
)

if exist "%GDB_PATH%" (
    echo [OK] GDB already installed
    "%GDB_PATH%" --version
    exit /b 0
)

echo [INFO] Installing GDB...
"%BASH%" -lc "pacman -S --needed --noconfirm mingw-w64-x86_64-gdb" || exit /b 1

if not exist "%GDB_PATH%" (
    echo [ERROR] GDB installation failed
    exit /b 1
)

echo [OK] GDB installed successfully
"%GDB_PATH%" --version
exit /b 0
