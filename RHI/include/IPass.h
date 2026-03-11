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

    virtual void Execute() = 0;
};
#endif //!RHI_IPASS__H_
