#ifndef RHI_IBUFFER__H_
#define RHI_IBUFFER__H_

#include "Define.h"
#include "IObject.h"

#include <string>
#include <vector>

enum class DataPattern
{
    STATIC,
    STREAM,
    DYNAMIC,
};

template <typename T>
class IBuffer : public IObject  
{
public:
    virtual ~IBuffer() = 0;

    virtual void* Map(std::vector<T> data) = 0;
    virtual void Unmap() = 0;
    virtual void Bind() = 0;
    virtual void StoreData(std::vector<T> data, DataPattern dataType = DataPattern::STATIC) = 0;

protected:
    std::vector<T> m_data;
};

#endif //!RHI_IBUFFER__H_
