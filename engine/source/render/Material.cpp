#include "Material.h"
#include "Engine.h"
#include "graphics/ShaderProgram.h"
#include "graphics/Texture.h"
#include "nlohmann/json.hpp"
#include <iostream>

namespace eng
{
    ShaderProgram* Material::GetShaderProgram() const
    {
        return m_shaderProgram.get();
    }

    void Material::SetShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram)
    {
        m_shaderProgram = shaderProgram;
    }

    void Material::SetParam(const std::string& name, float value)
    {
        m_floatParams[name] = value;
    }

    void Material::SetParam(const std::string& name, float v0, float v1)
    {
        m_float2Params[name] = { v0, v1 };
    }

    void Material::SetParam(const std::string& name, std::shared_ptr<Texture>& texture)
    {
        m_textures[name] = texture;
    }

    void Material::Bind() const
    {
        if (!m_shaderProgram)
        {
            return;
        }
        
        m_shaderProgram->Bind();
        for (auto& param : m_floatParams)
        {
            m_shaderProgram->SetUniform(param.first, param.second);
        }

        for (auto& param : m_float2Params)
        {
            m_shaderProgram->SetUniform(param.first, param.second.first, param.second.second);
        }
        
        for (auto& param : m_textures)
        {
            m_shaderProgram->SetTexture(param.first, param.second.get());
        }
    }

    std::shared_ptr<Material> Material::Load(const std::string& path)
    {
        auto data = Engine::GetInstance().GetFileSystem().LoadAssetFileText(path);
        
        if (data.empty())
        {
            return nullptr;
        }
        
        nlohmann::json json;
        try
        {
            json = nlohmann::json::parse(data);
        }
        catch (const nlohmann::json::parse_error& error)
        {
            std::cerr << "Failed to parse material '" << path << "': " << error.what() << '\n';
            return nullptr;
        }
        
        std::shared_ptr<eng::Material> result;
        
        if (json.contains("shader"))
        {
            auto shaderObj = json["shader"];
            std::string vertexPath = shaderObj.value("vertex", "");
            std::string fragmentPath = shaderObj.value("fragment", "");
            
            auto& filesystem = Engine::GetInstance().GetFileSystem();
            auto vertexSrc = filesystem.LoadAssetFileText(vertexPath);
            auto fragmentSrc = filesystem.LoadAssetFileText(fragmentPath);
            
            auto& graphicsAPI = Engine::GetInstance().GetGraphicsAPI();
            auto shaderProgram = graphicsAPI.CreateShaderProgram(vertexSrc, fragmentSrc);
            if (!shaderProgram)
            {
                return nullptr;
            }
            
            result = std::make_shared<eng::Material>();
            result->SetShaderProgram(shaderProgram);
        }
        
        if (json.contains("params"))
        {
            if (!result)
            {
                std::cerr << "Failed to load material '" << path << "': missing or invalid shader\n";
                return nullptr;
            }
            
            auto paramsObj = json["params"];
            
            //floats
            if (paramsObj.contains("float"))
            {
                for (auto& floatParam : paramsObj["float"])
                {
                    std::string name = floatParam.value("name", "");
                    float value = floatParam.value("value", 0.0f);
                    result->SetParam(name, value); 
                }
            }
            
            //float2
            if (paramsObj.contains("float2"))
            {
                for (auto& floatParam : paramsObj["float2"])
                {
                    std::string name = floatParam.value("name", "");
                    float value0 = floatParam.value("value0", 0.0f);
                    float value1 = floatParam.value("value1", 0.0f);
                    result->SetParam(name, value0, value1);
                }
            }
            
            //textures
            if (paramsObj.contains("textures"))
            {
                for (auto& textureParam : paramsObj["textures"])
                {
                    std::string name = textureParam.value("name", "");
                    std::string texturePath = textureParam.value("path", textureParam.value("value", ""));
                    auto texture = Texture::LoadFromFile(texturePath);
                    if (!texture)
                    {
                        std::cerr << "Failed to load texture '" << texturePath << "' for material '" << path << "'\n";
                        continue;
                    }
                    
                    result->SetParam(name, texture); 
                }
            }
        }
        
        return result;
    }
}
