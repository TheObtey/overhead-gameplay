#ifndef ORE_TEXTURE_OBJECT__H_
#define ORE_TEXTURE_OBJECT__H_

#include "ITextureObject.h"

#include <glad/glad.h>

enum class DataType
{
    UBYTE = GL_UNSIGNED_BYTE,
    FLOAT = GL_FLOAT
};

enum class TextureType
{
    TYPE_1D = GL_TEXTURE_1D,
    TYPE_2D = GL_TEXTURE_2D,
    TYPE_3D = GL_TEXTURE_3D,
};

class TextureObject final : public ITextureObject
{
public:
    TextureObject(uint32 id, TextureType type);
    ~TextureObject() override;

    void Bind() override;

    void GenerateTextureImage(DataType type, uint32& width, uint32& height, std::string imagePath = "");
    void AddParameters(uint32 parameter, uint32 value) override;
    void AttachToFrameBuffer(uint32 frameBuffer, uint32 attachment) override;
    void InitializeTextureView(uint32 origTexture, TextureType format, uint32 minLevels, uint32 numlevels, uint32 minLayer, uint32 numLayers);

private:
    GLuint m_id;
    TextureType m_type;
};

#endif
