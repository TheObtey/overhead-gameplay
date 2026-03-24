#ifndef RHI_IBUFFER__H_
#define RHI_IBUFFER__H_

#include "Define.h"
#include "IObject.h"

template <typename T>
class IBuffer : public IObject  
{
public:
    virtual ~IBuffer() = 0;

    virtual T* Map(uint32 offset, uint32 size) = 0;
    virtual void Unmap() = 0;
    virtual void Bind() = 0;

protected:
    virtual void StoreData(std::vector<T>& data) = 0;

};

#endif //!RHI_IBUFFER__H_
