#ifndef ORE_SHADER__H_
#define ORE_SHADER__H_

#include "IShader.h"

enum class ShaderType
{
    TYPE_VERTEX = GL_VERTEX_SHADER,
    TYPE_FRAGMENT = GL_FRAGMENT_SHADER,
    TYPE_COMPUTE = GL_COMPUTE_SHADER
};

class Shader : public IShader
{
public:
    Shader(ShaderType type);
    ~Shader();

    void Load(std::filesystem::path const& path) override;
    void Unload() override;
    uint32 GetId() const override {return m_id;}

private:
    GLuint m_id;
    ShaderType m_type;
};
#endif
