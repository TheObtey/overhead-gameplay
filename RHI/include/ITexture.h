#ifndef RHI_ITEXTURE_H__
#define RHI_ITEXTURE_H__

#include <string>

class ITexture 
{
public:
    ITexture(std::string_view buffer);
    ~ITexture();

protected:
    int width;
    int height;
};

#endif //!RHI_ITEXTURE_H__
