#ifndef ORE_BUFFER__H_
#define ORE_BUFFER__H_

#include "IBuffer.h"

#include <glad/glad.h>

enum class BufferType
{
    BUFFER,  
    BUFFER_ELEMENT,
};


template<typename T>
class Buffer : public IBuffer<T>
{
public:
    Buffer(std::vector<T> data, uint32 id, BufferType type);
    ~Buffer() override;

    void Load() override;
    void Unload() override;

    T* Map() override;
    void Unmap() override;
    void Bind() override;
    void StoreData(std::vector<T> data) override;

private:
    std::vector<T> m_data;
    BufferType m_type;
    GLuint m_id;
};

#include "Buffer.inl"

#endif
