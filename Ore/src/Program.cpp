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
        Logger::LogWithLevel(LogLevel::ERROR, "Program is not linked");
    }
}

void Program::AddShader(IShader* pShader)
{
    m_shaderIds.push_back(static_cast<GLuint>(pShader->GetId()));
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
