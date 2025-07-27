@echo off
echo === Building Pulse Engine Editor to .exe window ===

g++  -Isrc -Iimgui -Iimgui/backends -Iinclude -I. ^
src/main.cpp ^
imgui/imgui.cpp ^
imgui/imgui_draw.cpp ^
imgui/imgui_tables.cpp ^
imgui/imgui_widgets.cpp ^
imgui/imgui_demo.cpp ^
imgui/backends/imgui_impl_glfw.cpp ^
imgui/backends/imgui_impl_opengl3.cpp ^
-LPulseEngineBuild -lPulseEngineEditor  ^
-LC:/path/to/glfw/lib -lglfw3 -lgdi32 -lopengl32 ^
-DENGINE_EDITOR ^
-DWINDOW_PULSE_EXPORT ^
-o PulseEngineBuild/PulseEditor.exe

if %errorlevel% neq 0 (
    echo Build failed!
    exit /b %errorlevel%
) else (
    echo Build succeeded!
)
