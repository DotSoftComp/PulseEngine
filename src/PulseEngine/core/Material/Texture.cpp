#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

Texture::Texture(const std::string &filePath)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Paramètres de texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Flip vertical selon besoin
    unsigned char* data = stbi_load((std::string(ASSET_PATH) + filePath).c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture: " << (std::string(ASSET_PATH) + filePath) << std::endl;
    }

    stbi_image_free(data);
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + static_cast<GLuint>(slot));
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
