#include "Texture.h"
#include <stb_image.h>
#include "Engine.h"

namespace eng
{
    Texture::Texture(int width, int height, int numChannels, unsigned char* data) 
        : m_Width(width), m_Height(height), m_NumChannels(numChannels)
    {
        Init(width, height, numChannels, data);
    }

    Texture::~Texture()
    {
        if (m_TextureID > 0)
        {
            glDeleteTextures(1, &m_TextureID);
        }
    }

    void Texture::Init(int width, int height, int numChannels, unsigned char* data)
    {
        GLint internalFormat = GL_RGB;
        GLenum format = GL_RGB;
        if (numChannels == 4)
        {
            internalFormat = GL_RGBA;
            format = GL_RGBA;
        }
        else if (numChannels == 1)
        {
            internalFormat = GL_RED;
            format = GL_RED;
        }

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    GLuint Texture::GetTextureID() const
    {
        return m_TextureID;
    }

    std::shared_ptr<Texture> Texture::LoadFromFile(const std::string& path)
    {
        int width, height, numChannels;
        auto& fileSystem = Engine::GetInstance().GetFileSystem();
        auto fullpath = fileSystem.GetAssetsFolder() / path;
        if (!std::filesystem::exists(fullpath))
        {
            return nullptr;
        }
        
        std::shared_ptr<Texture> result;
        unsigned char* data = stbi_load(fullpath.string().c_str(), &width, &height, &numChannels, 0);
        if (data)
        {
            result = std::make_shared<eng::Texture>(width, height, numChannels, data);
            stbi_image_free(data);
        }
        
        return result;
    }

    std::shared_ptr<Texture> TextureManager::GetOrLoadTexture(const std::string& path)
    {
        auto it = m_Textures.find(path);
        if (it != m_Textures.end())
        {
            return it->second;
        }
        
        auto texture = Texture::LoadFromFile(path);
        m_Textures[path] = texture;
        return texture;
    }
}
