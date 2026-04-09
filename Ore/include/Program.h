#ifndef ORE_PROGRAM__H_
#define ORE_PROGRAM__H_

#include "IProgram.h"
#include "Define.h"
#include "Ore.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat4x4.hpp>

namespace Ore
{
class Program : public IProgram
{
public:
    Program();
    ~Program();

    void AddShader(IShader const& pShader) override;
    void Load() override;

    void Use() override;

    uint32 GetProgramId() const override { return m_programId; }

    template <typename T>
    void SetUniform(std::string const& name, T value);

    void Unload() override;

private:
    GLuint m_programId;
    std::vector<GLuint> m_shaderIds;

};

template<>
inline void Program::SetUniform<int32>(const std::string &name, int32 value)
{
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}

template<>
inline void Program::SetUniform<uint32>(const std::string &name, uint32 value)
{
    glUniform1ui(glGetUniformLocation(m_programId, name.c_str()), value);
}

template<>
inline void Program::SetUniform<float>(const std::string &name, float value)
{
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

template<>
inline void Program::SetUniform<glm::vec2>(const std::string &name, glm::vec2 value)
{
    glUniform2fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

template<>
inline void Program::SetUniform<glm::vec3>(const std::string &name, glm::vec3 value)
{
    glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

template<>
inline void Program::SetUniform<glm::vec4>(const std::string &name, glm::vec4 value)
{
    glUniform4fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

template<>
inline void Program::SetUniform<glm::mat2>(const std::string &name, glm::mat2 value)
{
    glUniformMatrix2fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

template<>
inline void Program::SetUniform<glm::mat3>(const std::string &name, glm::mat3 value)
{
    glUniformMatrix3fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

template<>
inline void Program::SetUniform<glm::mat4>(const std::string &name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
}
#endif

