#ifndef RHI_IPROGRAM__H_
#define RHI_IPROGRAM__H_

#include <Define.h>

namespace Ore
{
class IShader;
class IProgram 
{
public:
    virtual void Load() = 0;
    virtual void Unload() = 0;

    virtual void AddShader(IShader const& pShader) = 0;

    virtual void Use() = 0;
    virtual uint32 GetProgramId() const = 0;

    //template <typename T>
    //virtual void SetUniform(std::string const& name, T value) = 0;
    //virtual void SetBool(std::string const& name, bool value) = 0;
    //virtual void SetInt(std::string const& name, int value) = 0;
    //virtual void SetFloat(std::string const& name, float value) = 0;

    //virtual void SetVec2(std::string const& name, glm::vec2 const& value) = 0;
    //virtual void SetVec3(std::string const& name, glm::vec3 const& value) = 0;
    //virtual void SetVec4(std::string const& name, glm::vec4 const& value) = 0;

    //virtual void SetMat2(std::string const& name, glm::mat2 const& value) = 0;
    //virtual void SetMat3(std::string const& name, glm::mat3 const& value) = 0;
    //virtual void SetMat4(std::string const& name, glm::mat4 const& value) = 0;
};
}
#endif //!RHI_ISHADER__H_
