#include "Program.h"
#include "Logger.hpp"
#include "IShader.h"

#include <filesystem>
#include <fstream>
#include <sstream>

Program::Program()
{
}

void Program::Load()
{
    m_programId = glCreateProgram();
    for(uint32 id : m_shaderIds)
    {
        glAttachShader(m_programId, id);
    }

    glLinkProgram(m_programId);

    GLint isLinked;
    glGetProgramiv(m_programId, GL_LINK_STATUS, (int*)&isLinked);
    if(isLinked == GL_FALSE)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Program is not linked : ");
        std::string log;
        log.resize(GL_INFO_LOG_LENGTH);
        glGetProgramInfoLog(m_programId, GL_INFO_LOG_LENGTH, nullptr, log.data());
        Logger::LogWithLevel(LogLevel::ERROR, log);
    }
}

void Program::AddShader(IShader const& pShader)
{
    m_shaderIds.push_back(pShader.GetId());
}

void Program::Use()
{
    glUseProgram(m_programId);
}

void Program::Unload()
{
    glDeleteProgram(m_programId);
}

Program::~Program()
{
}
