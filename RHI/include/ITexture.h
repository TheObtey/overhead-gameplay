#ifndef RHI_ITEXTURE_H__
#define RHI_ITEXTURE_H__

#include "Define.h"
#include "TextureObject.h"
#include <filesystem>

namespace Ore
{
enum class TextureMaterialType : uint8
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT
};

class ITexture 
{
public:
    virtual void Load(std::filesystem::path const& path, TextureType type, TextureMaterialType meterialType) = 0;
    virtual void Unload() = 0;
};
}
#endif //!RHI_ITEXTURE_H__
