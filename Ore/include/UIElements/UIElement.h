#ifndef ORE_UI_ELEMENT__H_
#define ORE_UI_ELEMENT__H_

#include "Define.h"
#include "Program.h"
#include "Texture.h"

#include <glm/glm.hpp>

namespace Ore
{
struct UIElement
{
    UIElement() = default;
    UIElement(uint32 _x, uint32 _y, uint32 _width, uint32 _height)
    {
        x = _x, y = _y, width = _width, height = _height;
    }

    UIElement(glm::uvec2 position, glm::uvec2 size)
    {
        x = position.x, y = position.y, width = size.x, width = size.y;
    }

    virtual void Draw(Program& program, int32& index) const = 0;

    uint32 width = 10;
    uint32 height = 10;
    uint32 x = 0;
    uint32 y = 0;
    float scale = 1;
    Color color = Color::SKY_BLUE;

    sptr<Texture> pTexture;
};
}
#endif
