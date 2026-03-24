#ifndef RHI_IPASS__H_
#define RHI_IPASS__H_

#include "IShader.h"
#include "ITexture.h"
#include "IGeometry.h"
#include "Define.h"

#include <variant>
#include <vector>


class IPass 
{
public:
    virtual ~IPass() = 0;

    virtual void AddShader(IShader const& shader) = 0;

    template <typename ... Args>
    void AddInput(Args ... args); 
    
    //virtual uptr<ITexture> Execute() = 0;
protected:

};
#endif //!RHI_IPASS__H_
