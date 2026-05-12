#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"

namespace eng
{
    ShaderProgram::ShaderProgram(GLuint ShaderProgramID) : m_ShaderProgramID(ShaderProgramID)
    {
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_ShaderProgramID);
    }

    void ShaderProgram::Bind()
    {
        glUseProgram(m_ShaderProgramID);
        m_currentTextureUnit = 0;
    }

    GLint ShaderProgram::GetUniformLocation(const std::string& name)
    {
        auto it = m_UniformLocationCache.find(name);
        if (it != m_UniformLocationCache.end())
        {
            return it->second;
        }
        
        GLint location = glGetUniformLocation(m_ShaderProgramID, name.c_str());
        m_UniformLocationCache[name] = location;
        return location;
    }

    void ShaderProgram::SetUniform(const std::string& name, float value)
    {
        auto location = GetUniformLocation(name);
        glUniform1f(location, value);
    }

    void ShaderProgram::SetUniform(const std::string& name, float v0, float v1)
    {
        auto location = GetUniformLocation(name);
        glUniform2f(location, v0, v1);   
    }

    void ShaderProgram::SetUniform(const std::string& name, glm::mat4 matrix)
    {
        auto location = GetUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));  
    }

    void ShaderProgram::SetTexture(const std::string& name, Texture* texture)
    {
        if (!texture)
        {
            return;
        }

        auto location = GetUniformLocation(name);
        
        glActiveTexture(GL_TEXTURE0 + m_currentTextureUnit);
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        glUniform1i(location, m_currentTextureUnit);
        ++m_currentTextureUnit;
    }
}
