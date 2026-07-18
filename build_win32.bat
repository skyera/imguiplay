@REM Build for Visual Studio compiler. Run your copy of vcvars32.bat or vcvarsall.bat to setup command-line compiler.
@set OUT_DIR=Debug
@set OUT_EXE=myimgui
@set INCLUDES=/Iexternal\imgui /Iexternal\imgui\backends /Iexternal\ImGuiFileDialog /Iexternal\implot /I..\imgui\examples\libs\glfw\include
@set SOURCES=main.cpp cadmodel.cpp external\imgui\imgui.cpp external\imgui\imgui_demo.cpp external\imgui\imgui_draw.cpp external\imgui\imgui_tables.cpp external\imgui\imgui_widgets.cpp external\imgui\backends\imgui_impl_glfw.cpp external\imgui\backends\imgui_impl_opengl2.cpp external\ImGuiFileDialog\ImGuiFileDialog.cpp external\implot\implot.cpp external\implot\implot_demo.cpp external\implot\implot_items.cpp
@set LIBS=/LIBPATH:..\imgui\examples\libs\glfw\lib-vc2010-32 glfw3.lib opengl32.lib gdi32.lib shell32.lib
mkdir %OUT_DIR%
cl /nologo /Zi /MD /D_CRT_SECURE_NO_WARNINGS %INCLUDES% %SOURCES% /Fe%OUT_DIR%/%OUT_EXE%.exe /Fo%OUT_DIR%/ /link %LIBS%
