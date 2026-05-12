#include "Texture.h"

namespace eng
{
    Texture::Texture(int width, int height, int numChannels, unsigned char* data) 
        : m_Width(width), m_Height(height), m_NumChannels(numChannels)
    {
        GLenum format = GL_RGB;
        if (numChannels == 4)
        {
            format = GL_RGBA;
        }
        else if (numChannels == 1)
        {
            format = GL_RED;
        }

        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    Texture::~Texture()
    {
        if (m_TextureID > 0)
        {
            glDeleteTextures(1, &m_TextureID);
        }
    }

    GLuint Texture::GetTextureID() const
    {
        return m_TextureID;
    }
}
