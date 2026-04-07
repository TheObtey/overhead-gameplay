#ifndef RHI_IRENDER_GRAPH__H_
#define RHI_IRENDER_GRAPH__H_

#include "Define.h"

class IRenderGraph 
{
public:
    virtual void Execute() = 0;

protected:
    virtual void CreateGBuffer(uint32 screenWidth, uint32 screenHeight) = 0;
};
#endif //!RHI_IRENDER_GRAPH__H_
