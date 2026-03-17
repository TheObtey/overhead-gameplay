#include "Shader.h"
#include "Logger.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

Shader::Shader(std::string const& vertBinaryPath, std::string const& fragBinaryPath)
{
    std::string vertexBinary;
    std::string fragmentBinary;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    if(!std::filesystem::exists(vertBinaryPath))
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Vertex shader at path : ", vertBinaryPath, " file doesn't exist");
        return;
    }
 
    if(!std::filesystem::exists(fragBinaryPath))
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Fragment shader at path : ", fragBinaryPath, " file doesn't exist");
        return;
    }   

    vShaderFile.open(vertBinaryPath);
    fShaderFile.open(fragBinaryPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();		

    vShaderFile.close();
    fShaderFile.close();
    vertexBinary = vShaderStream.str();
    fragmentBinary = fShaderStream.str();	

    Logger::Log("Start Shader");

    const char* vertexBinaryCode = vertexBinary.c_str();
    const char* fragmentBinaryCode = fragmentBinary.c_str();
    uint32 vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderBinary(1, &vertex, GL_SHADER_BINARY_FORMAT_SPIR_V, vertexBinary.c_str(), vertexBinary.size());

    std::string vsEntryPoint = "main";
    glSpecializeShader(vertex, static_cast<const GLchar*>(vsEntryPoint.c_str()), 0, nullptr, nullptr);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderBinary(1, &fragment, GL_SHADER_BINARY_FORMAT_SPIR_V, fragmentBinary.c_str(), fragmentBinary.size());

    std::string fsEntryPoint = "main";
    glSpecializeShader(fragment, static_cast<const GLchar*>(fsEntryPoint.c_str()), 0, nullptr, nullptr);

    GLint isCompiled;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
        Logger::LogWithLevel(LogLevel::ERROR, "Shader is not compiled");

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
        Logger::LogWithLevel(LogLevel::ERROR, "Shader is not compiled");


    m_programId = glCreateProgram();
    glAttachShader(m_programId, vertex);
    glAttachShader(m_programId, fragment);

    glLinkProgram(m_programId);

    GLint isLinked;
    glGetProgramiv(m_programId, GL_LINK_STATUS, (int*)&isLinked);
    if(isLinked == GL_FALSE)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Program is not linked");
    }

    glDetachShader(m_programId, vertex);
    glDetachShader(m_programId, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use()
{
    glUseProgram(m_programId);
}

Shader::~Shader()
{
}

void Shader::SetBool(std::string const& name, bool value)
{
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), (int) value);
}

void Shader::SetInt(std::string const& name, int value)
{
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::SetFloat(std::string const& name, float value)
{
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::SetVec2(std::string const& name, glm::vec2 const& value)
{
    glUniform2fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(std::string const& name, glm::vec3 const& value)
{
    glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(std::string const& name, glm::vec4 const& value)
{
    glUniform4fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

void Shader::SetMat2(std::string const& name, glm::mat2 const& value)
{
    glUniformMatrix2fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat3(std::string const& name, glm::mat3 const& value)
{
    glUniformMatrix3fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat4(std::string const& name, glm::mat4 const& value)
{
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
