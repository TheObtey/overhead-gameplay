#ifndef ORE_BUFFER__H_
#define ORE_BUFFER__H_

#include "IBuffer.h"
#include "Ore.h"
#include <span>

namespace Ore
{
enum class BufferType
{
    BUFFER = GL_ARRAY_BUFFER, 
    BUFFER_ELEMENT = GL_ELEMENT_ARRAY_BUFFER,
    BUFFER_INDIRECT = GL_DRAW_INDIRECT_BUFFER,
    PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER 
};

template<typename T>
class Buffer final : public IBuffer<T>
{
public:
    Buffer(std::span<T const> data, uint32 id, BufferType type, bool isDataPersistant);
    Buffer(Buffer<T> const& other);
    ~Buffer() = default;

    void Bind() override;

    T* Map(uint32 offset, uint32 size) override;
    void Unmap() override;

protected:
    void StoreData(std::span<T const> data) override;

private:
    BufferType m_type;
    GLuint m_id;
    int m_dataPersistanceFlag;
};

#include "Buffer.inl"

}
#endif
