#pragma once
#include "GL/glew.h"

namespace eng
{
    class Texture
    {
    public:
        Texture(int width, int height, int numChannels, unsigned char* data);
        ~Texture();
        
        GLuint GetTextureID() const;
        
    private:
        GLuint m_TextureID = 0;
        int m_Width = 0;
        int m_Height = 0;
        int m_NumChannels = 0;
    };
}
