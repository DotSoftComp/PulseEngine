@echo off
setlocal enabledelayedexpansion

REM Set the output file
set OUTPUT=src/Engine.h

REM Write the header guard and comment
echo // Auto-generated include file > %OUTPUT%
echo #pragma once >> %OUTPUT%
echo. >> %OUTPUT%

REM Find all .h files in ./src (excluding Engine.h itself)
REM Ensure the src directory exists before searching
if exist "%~dp0src" (
    for /r "%~dp0src" %%f in (*.h) do (
        if /I not "%%~nxf"=="Engine.h" (
            set "relpath=%%f"
            set "relpath=!relpath:%~dp0=!"
            REM Remove "src\" from the beginning of the path if present
            if /I "!relpath:~0,4!"=="src\" (
                set "relpath=!relpath:~4!"
            )
            echo #include "!relpath!" >> %OUTPUT%
        )
    )
) else (
    echo Directory "%~dp0src" does not exist.
)

REM Find all .h files in ./PulseEngineEditor
if exist "%~dp0PulseEngineEditor" (
    for /r "%~dp0PulseEngineEditor" %%f in (*.h) do (
        REM Get the relative path from the PulseEngineEditor directory
            set "relpath=%%f"
            set "relpath=!relpath:%~dp0=!"
        echo #include "!relpath!" >> %OUTPUT%
    )
) else (
    echo Directory "%~dp0PulseEngineEditor" does not exist.
)

echo using namespace PulseEngine; >> %OUTPUT%

echo Done. Engine.h updated.