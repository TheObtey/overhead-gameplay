#ifndef RHI_IBUFFER__H_
#define RHI_IBUFFER__H_

#include "IObject.h"
#include "Define.h"
#include <span>

namespace Ore
{
template <typename T>
class IBuffer : public IObject  
{
public:
    virtual T* Map(uint32 offset, uint32 size) = 0;
    virtual void Unmap() = 0;
    virtual void Bind() override = 0;

protected:
    virtual void StoreData(std::span<T const> data) = 0;
};
}
#endif //!RHI_IBUFFER__H_
