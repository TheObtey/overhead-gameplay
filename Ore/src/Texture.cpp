#include "Texture.h"
#include "Logger.hpp"

Texture::Texture(std::string const& path, TextureType type, TextureMaterialType materialType)  
{

    Logger::Log("Start Texture creation");
    m_materialType = materialType;

    GLuint textureId;
    glGenTextures(1, &textureId);

    m_textureObject = TextureObject(textureId, type);

    uint32 width, height;
    m_textureObject.Bind();
    m_textureObject.GenerateTextureFromImage(DataType::UBYTE, width, height, path);
}


Texture::~Texture()
{
}
