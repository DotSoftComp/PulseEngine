#include "shader.h"
#include "Common/common.h"
#include "PulseEngine/core/Lights/Lights.h"
#include "PulseEngine/core/Lights/DirectionalLight/DirectionalLight.h"


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    // Charger et compiler les shaders
    std::string vertexCode = loadShaderCode(vertexPath);
    std::string fragmentCode = loadShaderCode(fragmentPath);
    
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
    
    shaderID = linkProgram(vertexShader, fragmentShader);
}

std::string Shader::loadShaderCode(const std::string& path) {
    std::ifstream shaderFile(path);
    if (!shaderFile.is_open())
    {
        std::cerr << "Erreur : impossible d'ouvrir le fichier shader : " << path << std::endl;
        return ""; // Return empty to prevent further problems
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
}

unsigned int Shader::compileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Erreur de compilation du " 
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
                  << " shader:\n" << infoLog << std::endl;
    }

    return shader;
}

unsigned int Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Erreur de linkage du shader program:\n" << infoLog << std::endl;
    }

    return program;
}

void Shader::Use() const
{
    glUseProgram(shaderID);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
    int location = glGetUniformLocation(shaderID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetVec3(const std::string &name, const glm::vec3 vec) const
{
    int location = glGetUniformLocation(shaderID, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

void Shader::SetFloat(const std::string &name, float fl) const
{
    int location = glGetUniformLocation(shaderID, name.c_str());
    glUniform1f(location, fl);
}

void Shader::SetBool(const std::string &name, bool b) const
{
    int location = glGetUniformLocation(shaderID, name.c_str());
    glUniform1i(location, static_cast<int>(b));
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::SetIntArray(const std::string& name, const int* values, int count) const
{
    glUniform1iv(glGetUniformLocation(shaderID, name.c_str()), count, values);
}

void Shader::SetLightingData(const std::vector<DirectionalLight*>& data) const
{
    int numLights = data.size();
    if (numLights == 0) return;

    // Prepare arrays
    std::vector<glm::vec3> positions(numLights);
    std::vector<glm::vec3> colors(numLights);
    std::vector<float> intensities(numLights);
    std::vector<float> attenuations(numLights);

    //Shadow data (currently commented out)
    std::vector<int> shadowCasterIndices;
    int shadowTextureBase = 5; // TEXTURE5, 6, 7...

    for (int i = 0; i < numLights; ++i)
    {
        positions[i] = glm::vec3(data[i]->GetPosition().x, data[i]->GetPosition().y, data[i]->GetPosition().z);
        colors[i] = glm::vec3(data[i]->color.r, data[i]->color.g, data[i]->color.b);
        intensities[i] = data[i]->intensity;
        attenuations[i] = data[i]->attenuation;
        SetInt("lightTypes[" + std::to_string(i) + "]", static_cast<int>(0));

        // If you have shadow casters, make sure to process them
        if (data[i]->castsShadow)
        {
            int casterIndex = static_cast<int>(shadowCasterIndices.size());
            shadowCasterIndices.push_back(i);
    
            std::string matName = "lightSpaceMatrices[" + std::to_string(casterIndex) + "]";
            SetMat4(matName.c_str(), data[i]->lightSpaceMatrix);
    
            glActiveTexture(GL_TEXTURE0 + casterIndex);
            glBindTexture(GL_TEXTURE_2D, data[i]->depthMapTex);
    
            std::string texName = "shadowMaps[" + std::to_string(casterIndex) + "]";
            SetInt(texName.c_str(), casterIndex); // matches GL_TEXTURE0 + i


        }
    }

    // Upload light arrays
    glUniform3fv(glGetUniformLocation(shaderID, "lightPositions"), numLights, glm::value_ptr(positions[0]));
    glUniform3fv(glGetUniformLocation(shaderID, "lightColors"), numLights, glm::value_ptr(colors[0]));
    glUniform1fv(glGetUniformLocation(shaderID, "lightIntensities"), numLights, intensities.data());
    glUniform1fv(glGetUniformLocation(shaderID, "lightAttenuations"), numLights, attenuations.data());
    glUniform1i(glGetUniformLocation(shaderID, "lightCount"), numLights);

    //Upload shadow caster indices (commented out, no shadows for now)
    if (!shadowCasterIndices.empty())
    {
        SetInt("shadowCasterCount", static_cast<int>(shadowCasterIndices.size()));
        SetIntArray("shadowCasterIndices", shadowCasterIndices.data(), static_cast<int>(shadowCasterIndices.size()));
    }
}

