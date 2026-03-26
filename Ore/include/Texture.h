#ifndef ORE_TEXTURE__H_
#define ORE_TEXTURE__H_

#include "ITexture.h"
#include "TextureObject.h"

enum class TextureMaterialType
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT
};

class Texture final : public ITexture
{
public:
    Texture(std::string const& path, TextureType type, TextureMaterialType meterialType);    
    ~Texture() override;

    TextureObject& GetTextureObject() { return m_textureObject; }
    TextureMaterialType GetTextureMaterialType() { return m_materialType; }

private:
    TextureObject m_textureObject;
    TextureMaterialType m_materialType;
     
};

#endif
