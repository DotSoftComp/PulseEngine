@echo off
echo === Compilation of PulseEngine.dll for Window ===

g++ -shared ^
-o Build/PulseEngine.dll ^
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
src/PulseEngine/core/Lights/PointLight/PointLight.cpp ^
src/PulseEngine/core/Material/Texture.cpp ^
src/PulseEngine/core/Lights/LightManager.cpp ^
src/PulseEngine/core/Physics/CollisionManager.cpp ^
src\PulseEngine\core\coroutine\CoroutineManager.cpp ^
PulseEngineEditor\InterfaceEditor\BuildGameCoroutine.cpp ^
src\PulseEngine\ModuleLoader\ModuleLoader.cpp ^
src\PulseEngine\API\EngineApi.cpp ^
src\PulseEngine\API\GameEntity.cpp ^
src\PulseEngine\core\Math\Transform\Transform.cpp ^
-I. ^
-Iinclude ^
-I./src -Iexternal/assimp/include ^
-ID:/msys64/mingw64/include ^
-IE:/Editor/Include ^
-Lexternal/assimp/lib ^
-Lexternal/assimp/lib/x64 ^
-L D:/MSYS2/mingw64/lib ^
-lglfw3 ^
-lopengl32 ^
-lm -lassimp ^
-DBUILDING_DLL ^
-DWINDOW_PULSE_EXPORT

if %errorlevel% neq 0 (
  echo ❌ Erreur de compilation.
  exit /b %errorlevel%
) else (
  echo ✅ Compilation réussie de PulseEngine.dll
)
