#include "ShaderProgram.h"

namespace eng
{
    ShaderProgram::ShaderProgram(GLuint ShaderProgramID) : m_ShaderProgramID(ShaderProgramID)
    {
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_ShaderProgramID);
    }

    void ShaderProgram::Bind() const
    {
        glUseProgram(m_ShaderProgramID);
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
}
