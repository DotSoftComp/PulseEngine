

#include "Common/common.h"


#include "shader.h"
#include "camera.h"
#include "PulseEngine/core/PulseEngineBackend.h" 
#include "PulseEngine/core/Meshes/Mesh.h"
#include "PulseEngine/core/Entity/Entity.h"
#include "PulseEngine/core/Material/MaterialManager.h"

#include "PulseEngine/core/Material/Material.h"
#include "PulseEngine/core/Meshes/primitive/Primitive.h"
#include "PulseEngine/CustomScripts/IScripts.h"

#ifdef ENGINE_EDITOR
    #include "PulseEngineEditor/InterfaceEditor/InterfaceEditor.h"
#endif

#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <map>


int main()
{
    PulseEngineBackend* engine = new PulseEngineBackend();

    //during the compilation of the game, some datas are defined in the preprocessor.
    //here, we get them and use them with the engine. (the dll didnt have them, so we need to set them manually)
    #ifdef GAME_NAME
        std::cout << std::string(GAME_NAME) << std::endl;
        engine->SetGameName(GAME_NAME);
    #endif
    #ifdef GAME_VERSION
        std::cout << std::string(GAME_VERSION) << std::endl;
        engine->SetGameVersion(GAME_VERSION);
    #endif
    if (engine->Initialize() != 0)
    {
        std::cerr << "Engine failed to initialize" << std::endl;
        return -1;
    }

    
    EDITOR_ONLY(
        InterfaceEditor* editor = new InterfaceEditor(engine);
    )
    

    // === Boucle de rendu ===
    while (engine->IsRunning())
    {
        engine->PollEvents();
        engine->Update();

        engine->RenderShadow();
        engine->Render();
        EDITOR_ONLY(
            editor->Render(engine);
        )

        glfwSwapBuffers(engine->GetWindowContext()->GetGLFWWindow());
        glfwPollEvents();
    }
    engine->Shutdown();

    EDITOR_ONLY(
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    )  
    delete engine;
    #ifdef ENGINE_EDITOR
        delete editor;
    #endif

    return 0;
}
