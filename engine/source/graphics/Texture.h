#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "GL/glew.h"

namespace eng
{
    class Texture
    {
    public:
        Texture(int width, int height, int numChannels, unsigned char* data);
        ~Texture();
        void Init(int width, int height, int numChannels, unsigned char* data);
        
        GLuint GetTextureID() const;
        
        static std::shared_ptr<Texture> LoadFromFile(const std::string& path);
        
    private:
        GLuint m_TextureID = 0;
        int m_Width = 0;
        int m_Height = 0;
        int m_NumChannels = 0;
    };
    
    class TextureManager
    {
    public:
        std::shared_ptr<Texture> GetOrLoadTexture(const std::string& path);
        
    private:
        std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
    };
}
