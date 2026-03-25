#include "TextureObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Logger.hpp"

#include <dependencies/stb_image.h>

TextureObject::TextureObject(uint32 id, TextureType type)
{
    m_id = id;
    m_type = type;
}

TextureObject::~TextureObject()
{
}

void TextureObject::Bind()
{
    glBindTexture(static_cast<uint32>(m_type), m_id);
}

void TextureObject::GenerateTextureFromImage(DataType type, uint32& width, uint32& height, std::string imagePath)
{
    int32 nrChannels, imageWidth, imageHeight;
    unsigned char* data = stbi_load(imagePath.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
    if(!data)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Failed to load texture at pass : " + imagePath);
        stbi_image_free(data);
        return;
    }


    GLenum format;
    if(nrChannels == 1)
        format = GL_RED;
    else if(nrChannels == 3)
        format = GL_RGB;
    else if(nrChannels == 4)
        format = GL_RGBA;

    Bind();
    glTexImage2D((uint32)m_type, 0, format, imageWidth, imageHeight, 0, format, (uint32)type, data);
    glGenerateMipmap((uint32)m_type);

    glTexParameteri((uint32)m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri((uint32)m_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri((uint32)m_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri((uint32)m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
}

void TextureObject::GenerateTexture(DataType type, uint32 const& width, uint32 const& height, uint32 rgbType)
{
    glTexImage2D((uint32)m_type, 0, rgbType, width, height, 0, GL_RGBA,(uint32)type, NULL);
}

void TextureObject::AddParameters(uint32 parameter, uint32 value)
{
    glTexParameteri(static_cast<uint32>(m_type), parameter, value);
}

void TextureObject::AttachToFrameBuffer(uint32 frameBuffer, uint32 attachment)
{
    glFramebufferTexture2D(frameBuffer, attachment, static_cast<uint32>(m_type), m_id, 0);
}

void TextureObject::InitializeTextureView(uint32 origTexture, TextureType format, uint32 minLevels, uint32 numlevels, uint32 minLayer, uint32 numLayers)
{
    glTextureView(m_id, static_cast<uint32>(m_type), origTexture, static_cast<uint32>(format), minLevels, numlevels, minLayer, numLayers); 
}
