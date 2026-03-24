#ifndef ORE_BUFFER__H_
#define ORE_BUFFER__H_

#include "IBuffer.h"

#include <glad/glad.h>

enum class BufferType
{
    BUFFER = GL_ARRAY_BUFFER, 
    BUFFER_ELEMENT = GL_ELEMENT_ARRAY_BUFFER,
};

template<typename T>
class Buffer final : public IBuffer<T>
{
public:
    Buffer(std::vector<T> const& data, uint32 id, BufferType type, bool isDataPersistant);
    ~Buffer() override;

    void Bind() override;

    T* Map(uint32 offset, uint32 size) override;
    void Unmap() override;

protected:
    void StoreData(std::vector<T> const& data) override;

private:
    BufferType m_type;
    GLuint m_id;
    int m_dataPersistanceFlag;
};

#include "Buffer.inl"

#endif
