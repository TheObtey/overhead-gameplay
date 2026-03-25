#ifndef ORE_SHADER__H_
#define ORE_SHADER__H_

#include "IShader.h"
#include "Define.h"

class Shader : public IShader
{
public:
    Shader(std::string const& vertPath, std::string const& fragPath);
    ~Shader() override;

    void Use() override;
    uint32 GetProgramId() { return m_programId; }

    void SetBool(std::string const& name, bool value) override;
    void SetInt(std::string const& name, int value) override;
    void SetFloat(std::string const& name, float value) override;

    void SetVec2(std::string const& name, glm::vec2 const& value) override;
    void SetVec3(std::string const& name, glm::vec3 const& value) override;
    void SetVec4(std::string const& name, glm::vec4 const& value) override;

    void SetMat2(std::string const& name, glm::mat2 const& value) override;
    void SetMat3(std::string const& name, glm::mat3 const& value) override;
    void SetMat4(std::string const& name, glm::mat4 const& value) override;

private:
    uint32 m_programId;

};
#endif
