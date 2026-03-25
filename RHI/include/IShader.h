#ifndef RHI_ISHADER__H_
#define RHI_ISHADER__H_

#include <string>
#include <glm/glm.hpp>

class IShader 
{
public:
    virtual ~IShader() = 0;

    virtual void Use() = 0;

    virtual void SetBool(std::string const& name, bool value) = 0;
    virtual void SetInt(std::string const& name, int value) = 0;
    virtual void SetFloat(std::string const& name, float value) = 0;

    virtual void SetVec2(std::string const& name, glm::vec2 const& value) = 0;
    virtual void SetVec3(std::string const& name, glm::vec3 const& value) = 0;
    virtual void SetVec4(std::string const& name, glm::vec4 const& value) = 0;

    virtual void SetMat2(std::string const& name, glm::mat2 const& value) = 0;
    virtual void SetMat3(std::string const& name, glm::mat3 const& value) = 0;
    virtual void SetMat4(std::string const& name, glm::mat4 const& value) = 0;
};

inline IShader::~IShader() {}

#endif //!RHI_ISHADER__H_
