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
    Image(sptr<Texture> _pTexture)
    {
        pTexture = _pTexture;
    }

    void Draw(Program& program) override
    {
        program.SetUniform("texture_diffuse", 0);
        pGeometry->Draw();
    }
    
    uptr<Geometry> pGeometry = nullptr;
    sptr<Texture> pTexture = nullptr; 
};
}
#endif

