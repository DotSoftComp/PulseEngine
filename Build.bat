@echo off
setlocal

set SRC=src\main.cpp src\glad.c src\PulseEngine\core\GUID\GuidGenerator.cpp src\shader.cpp src\camera.cpp src\PulseEngine\core\PulseEngineBackend.cpp  src\PulseEngine\core\Inputs\Mouse.cpp src\PulseEngine\core\Entity\Entity.cpp src\PulseEngine\core\Meshes\mesh.cpp  src\PulseEngine\core\WindowContext\WindowContext.cpp src/PulseEngine/core/Material/Material.cpp src/PulseEngine/core/Material/MaterialManager.cpp src\PulseEngine\core\Meshes\primitive\Primitive.cpp src\PulseEngine\core\GUID\GuidReader.cpp src\PulseEngine\core\SceneLoader\SceneLoader.cpp
set INCLUDE=-Iinclude -I./src -IE:\Editor\Include
set LIBS=-L"D:\MSYS2\mingw64\lib" -lglfw3 -lopengl32 -lm

g++ %SRC% %INCLUDE% %LIBS% -o PulseEngine