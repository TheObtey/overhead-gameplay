#include "Shader.h"

#include <Logger.hpp>
#include <fstream>

Shader::Shader(ShaderType type) : m_type(type), m_id(0)
{
}

Shader::~Shader()
{
}

void Shader::Load(std::filesystem::path const& path)
{
    Logger::Log("Start Shader");

    std::string shader;
    std::ifstream shaderFile;

    if(!std::filesystem::exists(path))
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Shader at path : ", path, " file doesn't exist");
        return;
    }
    
    shaderFile.open(path);
    std::stringstream shaderStream;

    shaderStream << shaderFile.rdbuf();

    shaderFile.close();
    shader = shaderStream.str();

    const char* shaderCode = shader.c_str();

    m_id = glCreateShader(static_cast<GLuint>(m_type));

    glShaderSource(m_id, 1, &shaderCode, NULL);
    glCompileShader(m_id);

    GLint isCompiled;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
        Logger::LogWithLevel(LogLevel::ERROR, "Shader is not compiled");
}

void Shader::Unload()
{
    glDeleteShader(m_id);
}
