#include "PointLight.h"
#include "PulseEngine/core/Entity/Entity.h"

PointLight::PointLight(PulseEngine::Vector3 position, PulseEngine::Color color, float intensity, float attenuation, float farPlane, int shadowResolution)
    : LightData(position, color, intensity, attenuation),
      farPlane(farPlane),
      shadowResolution(DEFAULT_SHADOW_MAP_RES)
{
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     DEFAULT_SHADOW_MAP_RES, DEFAULT_SHADOW_MAP_RES, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "PointLight: Initial framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    RecalculateLightSpaceMatrix();
}

void PointLight::RecalculateLightSpaceMatrix()
{
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
    glm::vec3 pos(position.x, position.y, position.z);

    shadowTransforms = {
        shadowProj * glm::lookAt(pos, pos + glm::vec3( 1, 0, 0), glm::vec3(0, -1, 0)),
        shadowProj * glm::lookAt(pos, pos + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
        shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
        shadowProj * glm::lookAt(pos, pos + glm::vec3(0,-1, 0), glm::vec3(0, 0,-1)),
        shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),
        shadowProj * glm::lookAt(pos, pos + glm::vec3(0, 0,-1), glm::vec3(0, -1, 0))
    };
}

const std::array<glm::mat4, 6>& PointLight::GetShadowTransforms() const
{
    return shadowTransforms;
}

void PointLight::BindToShader(Shader& shader, int index)
{
    std::string prefix = "pointLights[" + std::to_string(index) + "].";

    shader.SetVec3(prefix + "position", glm::vec3(position.x, position.y, position.z));
    shader.SetVec3(prefix + "color", glm::vec3(color.r, color.g, color.b));
    shader.SetFloat(prefix + "intensity", intensity);
    shader.SetFloat(prefix + "attenuation", attenuation);
    shader.SetInt(prefix + "castsShadow", 1); // 1 for true, 0 for false
    shader.SetFloat(prefix + "farPlane", farPlane);
}

void PointLight::RenderShadowMap(Shader &shader, PulseEngineBackend& scene)
{
    // if (!castsShadow) return;

    std::cout << " ============= render shadowmap for light point ============= " << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glViewport(0, 0, DEFAULT_SHADOW_MAP_RES, DEFAULT_SHADOW_MAP_RES);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT); 

    shader.Use();
    shader.SetVec3("lightPos", glm::vec3(position.x, position.y, position.z));
    shader.SetFloat("farPlane", farPlane);

    for (unsigned int i = 0; i < 6; ++i)
    {
        std::cout << "Rendering shadow map face: " << i << std::endl;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, depthCubeMap, 0);

        shader.SetMat4("shadowMatrix", shadowTransforms[i]);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cerr << "Framebuffer not complete for cube face " << i << std::endl;
            continue; // optionally skip this face
        }
        for (Entity* obj : scene.entities)
        {
            std::cout << "Drawing entity: " << obj->GetName() << std::endl;
            glm::mat4 model = glm::make_mat4(obj->GetMatrix().Ptr());
            shader.SetMat4("model", model);
            obj->DrawMeshWithShader(shader.getProgramID());
        }
    }

    glCullFace(GL_BACK); 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
