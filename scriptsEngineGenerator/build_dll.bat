@echo off
echo === Compilation of Pulse Engine.dll for Window ===

g++ -shared -Wl,--out-implib,PulseEngineBuild/libPulseEngineEditor.a ^
-o PulseEngineBuild/PulseEngineEditor.dll ^
src/main.cpp ^
src/glad.c ^
src/PulseEngine/core/GUID/GuidGenerator.cpp ^
src/shader.cpp ^
src/camera.cpp ^
src/PulseEngine/core/PulseEngineBackend.cpp ^
src/PulseEngine/core/Inputs/Mouse.cpp ^
src/PulseEngine/core/Entity/Entity.cpp ^
src/PulseEngine/core/Meshes/mesh.cpp ^
src/PulseEngine/core/WindowContext/WindowContext.cpp ^
src/PulseEngine/core/Material/Material.cpp ^
src/PulseEngine/core/Material/MaterialManager.cpp ^
src/PulseEngine/core/Meshes/primitive/Primitive.cpp ^
src/PulseEngine/core/GUID/GuidReader.cpp ^
src/PulseEngine/core/SceneLoader/SceneLoader.cpp ^
src/PulseEngine/core/FileManager/FileManager.cpp ^
src/PulseEngine/core/Lights/DirectionalLight/DirectionalLight.cpp ^
src/PulseEngine/CustomScripts/ScriptsLoader.cpp ^
src/PulseEngine/core/Graphics/OpenGLAPI/OpenGLApi.cpp ^
src/PulseEngine/core/Physics/Collider/BoxCollider.cpp ^
src/PulseEngine/core/Meshes/SkeletalMesh.cpp ^
PulseEngineEditor/InterfaceEditor/InterfaceEditor.cpp ^
PulseEngineEditor/InterfaceEditor/TopBar.cpp ^
src/PulseEngine/core/Lights/PointLight/PointLight.cpp ^
src/PulseEngine/core/Material/Texture.cpp ^
src/PulseEngine/core/Lights/LightManager.cpp ^
src/PulseEngine/core/Physics/CollisionManager.cpp ^
src\PulseEngine\core\coroutine\CoroutineManager.cpp ^
PulseEngineEditor\InterfaceEditor\BuildGameCoroutine.cpp ^
src\PulseEngine\ModuleLoader\ModuleLoader.cpp ^
imgui/imgui.cpp ^
imgui/imgui_draw.cpp ^
imgui/imgui_tables.cpp ^
imgui/imgui_widgets.cpp ^
imgui/imgui_demo.cpp ^
imgui/backends/imgui_impl_glfw.cpp ^
imgui/backends/imgui_impl_opengl3.cpp ^
-I. -Iinclude -Iimgui  -I./src -IE:/Editor/Include -Iexternal/assimp/include ^
-Lexternal/assimp/lib ^
-Lexternal/assimp/lib/x64 ^
-LD:/MSYS2/mingw64/lib ^
-lglfw3 -lopengl32 -lm -lassimp ^
-DBUILDING_DLL -DENGINE_EDITOR -DWINDOW_PULSE_EXPORT

if %errorlevel% neq 0 (
    echo DLL build failed!
    exit /b %errorlevel%
) else (
    echo DLL build succeeded!
)
