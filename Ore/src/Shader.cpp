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
    glShaderSource(vertex, 1, &vertexBinaryCode, NULL);
    glCompileShader(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentBinaryCode, NULL);
    glCompileShader(fragment);

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
        std::string log;
        log.resize(512);
        glGetProgramInfoLog(m_programId, 512, nullptr, log.data());
        Logger::LogWithLevel(LogLevel::ERROR, log);
    }

    //glDetachShader(m_programId, vertex);
    //glDetachShader(m_programId, fragment);

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
