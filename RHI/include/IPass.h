#ifndef RHI_IPASS__H_
#define RHI_IPASS__H_

#include "Define.h"

#include <variant>


class IPass 
{
public:
    virtual ~IPass() = 0;

    virtual void Execute() = 0;
    virtual void SetGBuffer(uint32 gbuffer) = 0;
};
inline IPass::~IPass() {};
#endif //!RHI_IPASS__H_
