#ifndef RHI_ITEXTURE_OBJECT__H_
#define RHI_ITEXTURE_OBJECT__H_

#include "IObject.h"
#include "Define.h"

namespace Ore
{
class ITextureObject : public IObject
{
public:
    virtual void Bind() override = 0;

    virtual void AddParameters(uint32 parameter, uint32 value) = 0;
    virtual void AttachToFrameBuffer(uint32 frameBuffer, uint32 attachment) = 0;
};
}
#endif //!RHI_ITEXTURE_OBJECT__H_
