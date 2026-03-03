#ifndef RHI_ISHADER__H_
#define RHI_ISHADER__H_

#include <string>
class IShader 
{
public:
    IShader(std::string_view vertBuffer, std::string_view fragBuffer);
    ~IShader();

protected:
    virtual void Compile() = 0;

};
#endif //!RHI_ISHADER__H_
