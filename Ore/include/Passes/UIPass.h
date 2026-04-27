#ifndef ORE_UI_PASS__H_
#define ORE_UI_PASS__H_

#include "Pass.h"
#include "UIElements/UIElement.h"
#include "Buffer.h"
#include "Geometry.h"
#include "VertexArrayObject.h"


namespace Ore
{
class UIPass : public Pass
{
public:
    UIPass(Program& program);
    UIPass(Program& program, Camera* pCamera);
    ~UIPass() = default;

    void Execute() override;
    void AddUIElement(UIElement const& element);
    
private:
    struct alignas(16) SubComponent
    {
        glm::mat4 model;
        glm::vec3 coords; 
        glm::vec3 tintColor;
    }; 

    std::vector<std::reference_wrapper<UIElement const>> m_elements;

    uptr<VertexArrayObject>     m_pVertexArray;
    uptr<Buffer<uint8>>         m_pIndicesBuffer;
    uptr<Buffer<Vertex>>        m_pVertexBuffer;

};
}
#endif
