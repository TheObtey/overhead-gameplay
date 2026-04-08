#ifndef RHI_IPASS__H_
#define RHI_IPASS__H_

#include "Define.h"

namespace Ore
{
class IPass 
{
public:
    virtual void Execute() = 0;
    virtual void SetGBuffer(uint32 gbuffer) = 0;
};
}
#endif //!RHI_IPASS__H_
