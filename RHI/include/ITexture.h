#ifndef RHI_ITEXTURE_H__
#define RHI_ITEXTURE_H__

#include <string>

class ITexture 
{
public:
    virtual ~ITexture() = 0;
    
};
inline ITexture::~ITexture() {};
#endif //!RHI_ITEXTURE_H__
