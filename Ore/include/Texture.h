#ifndef ORE_TEXTURE__H_
#define ORE_TEXTURE__H_

#include "ITexture.h"

namespace Ore
{
class Texture final : public ITexture
{
public:
    Texture(std::filesystem::path const& path, TextureType type, TextureMaterialType materialType);
    Texture(TextureObject const& textureObject, TextureMaterialType const& materialType) {m_textureObject = textureObject, m_materialType = materialType;}
    ~Texture();

    void Load(std::filesystem::path const& path, Ore::TextureType type, TextureMaterialType materialType) override;
    void Unload() override;

    TextureObject& GetTextureObject() { return m_textureObject; }
    TextureMaterialType GetTextureMaterialType() const { return m_materialType; }

private:
    TextureObject m_textureObject;
    TextureMaterialType m_materialType;
     
};
}
#endif
