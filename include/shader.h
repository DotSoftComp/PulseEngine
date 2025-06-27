#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>

class LightData;
class DirectionalLight;

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    unsigned int getProgramID() const { return shaderID; }
    void Use() const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;
    void SetVec3(const std::string& name, const glm::vec3 vec) const;
    void SetFloat(const std::string& name, float fl) const;
    void SetBool(const std::string& name, bool b) const;
    void SetInt(const std::string& name, int value) const;
    void SetIntArray(const std::string& name, const int* values, int count) const;
    void SetLightingData(const std::vector<DirectionalLight*>& data) const;

private:
    unsigned int shaderID;

    std::string loadShaderCode(const std::string& path);
    unsigned int compileShader(unsigned int type, const char* source);
    unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
};

#endif // SHADER_H
