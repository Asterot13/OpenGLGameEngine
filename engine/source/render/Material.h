#pragma once
#include <memory>
#include <unordered_map>
#include <string>

namespace eng
{
    class ShaderProgram;
    class Material
    {
    public:
        void SetShaderProgram(std::shared_ptr<ShaderProgram>& shaderProgram);
        void SetParam(const std::string& name, float value);
        void Bind() const;
        
    private:
        std::shared_ptr<ShaderProgram> m_shaderProgram;
        std::unordered_map<std::string, float> m_floatParams;
    };
}
