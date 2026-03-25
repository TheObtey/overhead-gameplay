#ifndef ORE_LIGHT_PASS__H_
#define ORE_LIGHT_PASS__H_

#include "Pass.h"
#include "Color.h"
#include "VertexArrayObject.h"
#include "Buffer.h"

struct Light
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    
    float constant = 1.0f;
    float linear = 0.7f;
    float quadratic = 1.8f;
    Color color = {1.0f, 1.0f, 1.0f, 1.0f};

    Light() = default;
};

class LightPass final : public Pass
{
public:
    LightPass(Program const& Program, std::vector<Light> const& lights, sptr<Camera> pCamera);
    ~LightPass();

    void SetLights(std::vector<Light> const& lights);
    void Execute() override;

private:
    void RenderQuad();
    void GenerateQuad();

private:
    std::vector<sptr<Light>> m_lights;
    uptr<VertexArrayObject> m_quadVAO;
    uptr<Buffer<float>> m_quadVBO;
    GLuint m_quadVAOId;
    GLuint m_quadVBOId;
};
#endif
 
