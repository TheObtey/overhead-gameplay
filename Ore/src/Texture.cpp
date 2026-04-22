#include "Texture.h"
#include "Logger.hpp"

using namespace Ore;
Texture::Texture(std::filesystem::path const& path, TextureType const type, TextureMaterialType const materialType)
{
    Load(path, type, materialType);
}

Texture::~Texture()
{
    Unload();
}

void Texture::Load(std::filesystem::path const& path, TextureType const type, TextureMaterialType const materialType)
{
    m_materialType = materialType;

    GLuint textureId;

    glGenTextures(1, &textureId);

    m_textureObject = TextureObject(textureId, type);

    uint32 width, height;
    m_textureObject.Bind();
    m_textureObject.GenerateTextureFromImage(DataType::UBYTE, width, height, path.string());
    Logger::Log("Loaded texture : ", path, " | With ID ", textureId);
}

void Texture::Unload()
{
    GLuint const id = m_textureObject.GetTextureID();
    //TODO FIX
    //glDeleteTextures(1, &id);
    Logger::Log("Texture : ", m_textureObject.GetTextureID(), " unloaded");
}
