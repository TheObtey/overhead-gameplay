#ifndef ORE_IMAGE__H_
#define ORE_IMAGE__H_

#include "UIElement.h"
#include "Texture.h"
#include "Geometry.h"

namespace Ore
{
struct Image final: public UIElement
{
    Image() = default;
    Image(uint32 x, uint32 y, uint32 width, uint32 height, sptr<Texture> _pTexture) : UIElement(x, y, width, height)
    {
        pTexture = _pTexture;
    }

    void Draw(Program& program, int32& index) const override
    {
        std::string indexStr = std::to_string(index);

        glm::vec2 offset = {x, y};
        //glm::vec2 size ={   static_cast<float>(element.width)  / static_cast<float>(m_screenWidth), static_cast<float>(element.height) / (static_cast<float>(m_screenHeight)) };
        glm::vec2 size = {width, height }; 

        program.SetUniform("offsets[" + indexStr + "]", offset);
        program.SetUniform("sizes[" + indexStr + "]", size);

        program.SetUniform("uvPos[" + indexStr + "][0]", glm::vec2(0.0f, 1.0f));
        program.SetUniform("uvPos[" + indexStr + "][1]", glm::vec2(1.0f, 1.0f));
        program.SetUniform("uvPos[" + indexStr + "][2]", glm::vec2(1.0f, 0.0f));
        program.SetUniform("uvPos[" + indexStr + "][3]", glm::vec2(0.0f, 0.0f));

        program.SetUniform("textures[" + indexStr + "]", index);
        glActiveTexture(GL_TEXTURE0 + index);
        pTexture->GetTextureObject().Bind();
        program.SetUniform("textureId[" + indexStr + "]", index);
        index++;
    }

    uptr<Geometry> pGeometry = nullptr;
};
}
#endif

