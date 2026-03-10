#include "TextureObject.h"
#include "Logger.hpp"

#include <glad/glad.h>
#include <dependencies/stb_image.h>

TextureObject::TextureObject(uint32 id, TextureType type)
{
    m_id = id;
    m_type = type;
}

void TextureObject::Bind()
{
    glBindTexture((int)m_type, m_id);
}

void TextureObject::GenerateTextureImage(DataType type, uint32& width, uint32& height, std::string imagePath)
{
    if(imagePath == "")
    {
        glTexImage2D((int)m_type, 0, GL_RGBA16F, width, height, 0, (int)type, GL_RGBA, NULL);
        return;
    }

    int32 nrChannels, imageWidth, imageHeight;
    unsigned char* data = stbi_load(imagePath.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
    if(data)
    {
        glTexImage2D((int)m_type, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap((int)m_type);
    }
    else
    {
        Logger::LogWithLevel(LogLevel::ERROR, "Failed to load texture : " + imagePath);
    }

    stbi_image_free(data);
}

void TextureObject::AddParameters(uint32 parameter, uint32 value)
{
    glTextureParameteri((int)m_type, parameter, value);
}

void TextureObject::AttachToFrameBuffer(uint32 frameBuffer, uint32 attachment)
{
    glFramebufferTexture2D(frameBuffer, attachment, (int)m_type, m_id, 0);
}

void TextureObject::InitializeTextureView(uint32 origTexture, TextureType format, uint32 minLevels, uint32 numlevels, uint32 minLayer, uint32 numLayers)
{
    glTextureView(m_id, uint(m_type), origTexture, (int)format, minLevels, numlevels, minLayer, numLayers); 
}
