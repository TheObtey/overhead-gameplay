#ifndef ORE_PROGRAM__H_
#define ORE_PROGRAM__H_

#include "IProgram.h"
#include "Define.h"

class Program : public IProgram
{
public:
    Program();
    ~Program();

    void AddShader(IShader* pShader) override;
    void Load() override;

    void Use() override;

    uint32 GetProgramId() const override { return m_programId; }

    template <typename T>
    void SetUniform(std::string const& name, T value);

    void Unload() override;

    //void SetBool(std::string const& name, bool value) override;
    //void SetInt(std::string const& name, int value) override;
    //void SetFloat(std::string const& name, float value) override;

    //void SetVec2(std::string const& name, glm::vec2 const& value) override;
    //void SetVec3(std::string const& name, glm::vec3 const& value) override;
    //void SetVec4(std::string const& name, glm::vec4 const& value) override;

    //void SetMat2(std::string const& name, glm::mat2 const& value) override;
    //void SetMat3(std::string const& name, glm::mat3 const& value) override;
    //void SetMat4(std::string const& name, glm::mat4 const& value) override;

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

#endif

