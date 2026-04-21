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

    void Draw(Program& program) override
    {
        program.SetUniform("texture_diffuse", 0);
        pGeometry->Draw();
    }
    
    uptr<Geometry> pGeometry = nullptr;
};
}
#endif

