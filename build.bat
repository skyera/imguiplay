@echo off
setlocal enabledelayedexpansion

REM Check for help arguments
if /I "%~1"=="help" goto show_help
if /I "%~1"=="/help" goto show_help
if /I "%~1"=="-help" goto show_help
if /I "%~1"=="--help" goto show_help
if /I "%~1"=="/h" goto show_help
if /I "%~1"=="-h" goto show_help
if /I "%~1"=="?" goto show_help
if /I "%~1"=="/?" goto show_help

REM Try to run cl. If it fails, locate and run vcvarsall.bat automatically
where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo Compiler cl.exe not in PATH. Searching for Visual Studio installation...
    
    set "VS_INSTALL_PATH="
    set "VCVARS_PATH="
    
    REM 1. Check vswhere.exe if it exists
    if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
        for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do (
            set "VS_INSTALL_PATH=%%i"
        )
    ) else if exist "%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe" (
        for /f "usebackq tokens=*" %%i in (`"%ProgramFiles%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath`) do (
            set "VS_INSTALL_PATH=%%i"
        )
    )
    
    if defined VS_INSTALL_PATH (
        set "VCVARS_PATH=!VS_INSTALL_PATH!\VC\Auxiliary\Build\vcvarsall.bat"
    ) else (
        REM 2. Hardcoded fallback checks for standard VS 2022 and 2019 paths
        if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
            set "VCVARS_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
        ) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
            set "VCVARS_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat"
        ) else if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
            set "VCVARS_PATH=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
        ) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" (
            set "VCVARS_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
        ) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" (
            set "VCVARS_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat"
        ) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" (
            set "VCVARS_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
        )
    )
    
    if defined VCVARS_PATH (
        if exist "!VCVARS_PATH!" (
            echo Setting up MSVC environment using: !VCVARS_PATH!
            call "!VCVARS_PATH!" x86
        ) else (
            echo Error: Found VS installation path but vcvarsall.bat is missing.
            exit /b 1
        )
    ) else (
        echo Error: Visual Studio installation could not be detected.
        echo Please run this script from a Developer Command Prompt.
        exit /b 1
    )
)

@set BUILD_TYPE=debug
@if /I "%~1"=="release" set BUILD_TYPE=release

@if "%BUILD_TYPE%"=="release" (
    @set OUT_DIR=Release
    @set CL_FLAGS=/O2 /MD /EHsc /D_CRT_SECURE_NO_WARNINGS /DNDEBUG
) else (
    @set OUT_DIR=Debug
    @set CL_FLAGS=/Zi /Od /MD /EHsc /D_CRT_SECURE_NO_WARNINGS /D_DEBUG
)

@echo Building %BUILD_TYPE% version...
@set OUT_EXE=myimgui
@set INCLUDES=/Iexternal\imgui /Iexternal\imgui\backends /Iexternal\ImGuiFileDialog /Iexternal\implot /I..\imgui\examples\libs\glfw\include
@set SOURCES=main.cpp cadmodel.cpp external\imgui\imgui.cpp external\imgui\imgui_demo.cpp external\imgui\imgui_draw.cpp external\imgui\imgui_tables.cpp external\imgui\imgui_widgets.cpp external\imgui\backends\imgui_impl_glfw.cpp external\imgui\backends\imgui_impl_opengl2.cpp external\ImGuiFileDialog\ImGuiFileDialog.cpp external\implot\implot.cpp external\implot\implot_demo.cpp external\implot\implot_items.cpp
@set LIBS=/LIBPATH:..\imgui\examples\libs\glfw\lib-vc2010-32 glfw3.lib opengl32.lib gdi32.lib shell32.lib
mkdir %OUT_DIR% 2>nul
cl /nologo %CL_FLAGS% %INCLUDES% %SOURCES% /Fe%OUT_DIR%/%OUT_EXE%.exe /Fo%OUT_DIR%/ /link %LIBS%
goto :eof

:show_help
echo Usage: build.bat [debug ^| release ^| help]
echo.
echo Options:
echo   debug     - Build Debug configuration (default).
echo   release   - Build Release configuration (optimized).
echo   help      - Show this help message.
exit /b 0
