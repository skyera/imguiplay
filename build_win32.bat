@echo off
setlocal enabledelayedexpansion

REM Try to run cl. If it fails, locate and run vcvarsall.bat automatically
where cl >nul 2>nul
if %errorlevel% neq 0 (
    echo Compiler cl.exe not in PATH. Searching for Visual Studio installation...
    
    set "VS_INSTALL_PATH="
    
    REM Check ProgramFiles(x86) installer path first
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
        if exist "!VS_INSTALL_PATH!\VC\Auxiliary\Build\vcvarsall.bat" (
            echo Setting up MSVC environment using: !VS_INSTALL_PATH!\VC\Auxiliary\Build\vcvarsall.bat
            call "!VS_INSTALL_PATH!\VC\Auxiliary\Build\vcvarsall.bat" x86
        ) else (
            echo Error: Found VS at !VS_INSTALL_PATH! but vcvarsall.bat is missing.
        )
    ) else (
        echo Error: Visual Studio installation could not be detected.
        echo Please run this script from a Developer Command Prompt.
        exit /b 1
    )
)

@set OUT_DIR=Debug
@set OUT_EXE=myimgui
@set INCLUDES=/Iexternal\imgui /Iexternal\imgui\backends /Iexternal\ImGuiFileDialog /Iexternal\implot /I..\imgui\examples\libs\glfw\include
@set SOURCES=main.cpp cadmodel.cpp external\imgui\imgui.cpp external\imgui\imgui_demo.cpp external\imgui\imgui_draw.cpp external\imgui\imgui_tables.cpp external\imgui\imgui_widgets.cpp external\imgui\backends\imgui_impl_glfw.cpp external\imgui\backends\imgui_impl_opengl2.cpp external\ImGuiFileDialog\ImGuiFileDialog.cpp external\implot\implot.cpp external\implot\implot_demo.cpp external\implot\implot_items.cpp
@set LIBS=/LIBPATH:..\imgui\examples\libs\glfw\lib-vc2010-32 glfw3.lib opengl32.lib gdi32.lib shell32.lib
mkdir %OUT_DIR% 2>nul
cl /nologo /Zi /MD /D_CRT_SECURE_NO_WARNINGS %INCLUDES% %SOURCES% /Fe%OUT_DIR%/%OUT_EXE%.exe /Fo%OUT_DIR%/ /link %LIBS%
