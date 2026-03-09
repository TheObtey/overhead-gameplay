#ifndef RHI_ITEXTURE_OBJECT__H_
#define RHI_ITEXTURE_OBJECT__H_

#include "IObject.h"
#include "Define.h"

enum class TextureType
{
    TYPE_1D,
    TYPE_2D,
    TYPE_3D,

    TYPE_COUNT
};

class ITextureObject : public IObject
{
public:
    virtual ~ITextureObject() = 0;

    virtual void Load() override = 0;
    virtual void Unload() override = 0;

    virtual void GenerateTextureImage(uint16 width, uint16 height, std::string_view imagePath = "") = 0;
    virtual void AddParameters(uint32 parameter, uint32 value) = 0;
    virtual void AttachToFrameBuffer(uint32 attachment) = 0;

protected:
    
};

#endif //!RHI_ITEXTURE_OBJECT__H_
