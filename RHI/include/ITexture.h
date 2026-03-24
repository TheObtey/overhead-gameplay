#ifndef RHI_ITEXTURE_H__
#define RHI_ITEXTURE_H__

#include <string>

class ITexture 
{
public:
    virtual ~ITexture() = 0;
    
protected:
    int width;
    int height;
};

#endif //!RHI_ITEXTURE_H__
