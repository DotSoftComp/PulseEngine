#include "DirectionalLight.h"
#include "PulseEngine/core/Entity/Entity.h"

void DirectionalLight::InitShadowMap(int resolution)
{
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMapTex);
    glBindTexture(GL_TEXTURE_2D, depthMapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalLight::RenderShadowMap(Shader &shader, PulseEngineBackend &scene)
{
    glViewport(0, 0, DEFAULT_SHADOW_MAP_RES, DEFAULT_SHADOW_MAP_RES);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT); 

    shader.Use();
    shader.SetMat4("shadowMatrix", lightSpaceMatrix);
    for (Entity* obj : scene.entities)
    {
        glm::mat4 glmMat = glm::make_mat4(obj->GetMatrix().Ptr());
        shader.SetMat4("model", glmMat);
        obj->DrawMeshWithShader(shader.getProgramID());
    }

    glCullFace(GL_BACK); 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalLight::BindToShader(Shader &shader, int index)
{
#ifdef WINDOW_PULSE_EXPORT
    
    glm::vec3 direction = glm::normalize(glm::vec3(target.x - position.x, target.y - position.y, target.z - position.z));
    
    shader.SetVec3("dirLight.direction", direction);
    shader.SetVec3("dirLight.color", glm::vec3(color.r, color.g, color.b));
    shader.SetFloat("dirLight.intensity", intensity);
    shader.SetBool("dirLight.castsShadow", true);
    shader.SetMat4("dirLight.lightSpaceMatrix", lightSpaceMatrix);
#endif
}

void DirectionalLight::RecalculateLightSpaceMatrix()
{
#ifdef WINDOW_PULSE_EXPORT
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
    glm::mat4 lightView = glm::lookAt(glm::vec3(position.x, position.y, position.z), target, glm::vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;
#endif
}
