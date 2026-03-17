#include "Texture.h"
#include "Logger.hpp"

Texture::Texture(std::string const& path, TextureType type, TextureMaterialType materialType)  
{

    Logger::Log("Start Texture creation");
    m_materialType = materialType;

    Logger::Log("a");
    GLuint textureId;
    glGenTextures(1, &textureId);

    Logger::Log("a");
    m_textureObject = TextureObject(textureId, type);

    uint32 width, height;
    Logger::Log("a");
    m_textureObject.Bind();
    Logger::Log("a");
    m_textureObject.GenerateTextureFromImage(DataType::UBYTE, width, height, path);
    Logger::Log("a");
}


Texture::~Texture()
{
}
