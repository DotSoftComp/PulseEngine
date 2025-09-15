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
PulseEngineEditor/InterfaceEditor/InterfaceAPI/PulseInterfaceAPI.cpp ^
src/PulseEngine/core/Lights/PointLight/PointLight.cpp ^
src/PulseEngine/core/Material/Texture.cpp ^
src/PulseEngine/core/Lights/LightManager.cpp ^
src/PulseEngine/core/Physics/CollisionManager.cpp ^
src/PulseEngine/core/coroutine/CoroutineManager.cpp ^
PulseEngineEditor/InterfaceEditor/BuildGameCoroutine.cpp ^
src/PulseEngine/ModuleLoader/ModuleLoader.cpp ^
src/PulseEngine/API/EngineApi.cpp ^
src/PulseEngine/API/GameEntity.cpp ^
src/PulseEngine/core/Math/Transform/Transform.cpp ^
PulseEngineEditor\InterfaceEditor\InterfaceEditorFileExplorer.cpp ^
PulseEngineEditor\InterfaceEditor\Synapse\Synapse.cpp ^
PulseEngineEditor\InterfaceEditor\NewFileCreator\NewFileManager.cpp ^
src\PulseEngine\core\GUID\GuidCollection.cpp ^
src\PulseEngine\core\ExecutableManager\ExecutableLauncher.cpp ^
src\PulseEngine\core\ExecutableManager\PulseExecutable.cpp ^
src\PulseEngine\core\ExecutableManager\ExecutableCommunication.cpp ^
src\PulseEngine\API\EntityAPI\EntityApi.cpp ^
imgui/imgui.cpp ^
imgui/imgui_draw.cpp ^
imgui/imgui_tables.cpp ^
imgui/imgui_widgets.cpp ^
imgui/imgui_demo.cpp ^
imgui/backends/imgui_impl_glfw.cpp ^
imgui/backends/imgui_impl_opengl3.cpp ^
imgui-node/imgui_node_editor.cpp ^
imgui-node/imgui_node_editor_api.cpp ^
imgui-node/imgui_canvas.cpp ^
imgui-node/crude_json.cpp ^
src\PulseEngine\core\Network\Request\Request.cpp ^
PulseEngineEditor/InterfaceEditor/Account/Account.cpp ^
-I. -Iinclude -Iinclude/websocketpp -Iinclude/asio -Iimgui -Iimgui-node -I./src -IE:/Editor/Include -Iexternal/assimp/include ^
-Lexternal/assimp/lib ^
-Lexternal/assimp/lib/x64 ^
-LD:/MSYS2/mingw64/lib ^
-lglfw3 -lopengl32 -lm -lassimp -lcomdlg32 -lws2_32 -lwinmm -lmswsock ^
-DBUILDING_DLL -DENGINE_EDITOR -DPULSE_GRAPHIC_OPENGL -DPULSE_WINDOWS -DIMGUI_IMPL_OPENGL_LOADER_GLAD

if %errorlevel% neq 0 (
    echo DLL build failed!
    exit /b %errorlevel%
) else (
    echo DLL build succeeded!
    if not exist distribuables (
        mkdir distribuables
    )
    copy /Y PulseEngineBuild\libPulseEngineEditor.a distribuables\
    copy /Y PulseEngineBuild\PulseEngineEditor.dll distribuables\
    
    if not exist PulseEngineBuild\dist\src (
        mkdir PulseEngineBuild\dist\src
    )
    if not exist PulseEngineBuild\dist\include (
        mkdir PulseEngineBuild\dist\include
    )
    xcopy /E /I /Y src PulseEngineBuild\dist\src
    copy /Y src\main.cpp PulseEngineBuild\dist\main.cpp
    xcopy /E /I /Y include PulseEngineBuild\dist\include
    
    del /S /Q PulseEngineBuild\dist\src\*.cpp
    del /S /Q PulseEngineBuild\dist\include\*.cpp


    if not exist ..\ModuleCreator\src (
        mkdir ..\ModuleCreator\src
    )
    if not exist ..\ModuleCreator\include (
        mkdir ..\ModuleCreator\include
    )

    if not exist ..\ModuleCreator\PulseEngineEditor (
        mkdir ..\ModuleCreator\PulseEngineEditor
    )

    copy /Y PulseEngineBuild\libPulseEngineEditor.a ..\ModuleCreator\lib

    xcopy /E /I /Y src ..\ModuleCreator\src
    xcopy /E /I /Y include ..\ModuleCreator\include
    xcopy /E /I /Y PulseEngineEditor ..\ModuleCreator\PulseEngineEditor

    del /S /Q ..\ModuleCreator\src\*.cpp
    del /S /Q ..\ModuleCreator\include\*.cpp
    del /S /Q ..\ModuleCreator\PulseEngineEditor\*.cpp

)

