#include "Buffer.h" 

#include <glad/glad.h>

template <typename T>
Buffer<T>::Buffer(std::vector<T> const& data, uint32 id, BufferType type, bool isDataPersistant)
{
    m_id = id;
    m_type = type;    
    isDataPersistant ? m_dataPersistanceFlag = GL_MAP_PERSISTENT_BIT|GL_MAP_COHERENT_BIT : m_dataPersistanceFlag = GL_DYNAMIC_STORAGE_BIT;
    StoreData(data);
}

template <typename T>
void Buffer<T>::Bind()
{
    glBindBuffer((int)m_type, m_id);
}

template<typename T>
T* Buffer<T>::Map(uint32 offset, uint32 size)
{
    void* ptr = glMapBufferRange((int)m_type, offset, size, m_dataPersistanceFlag);
    return ptr;
}

template<typename T>
void Buffer<T>::Unmap()
{    
    glUnmapBuffer((int)m_type);
}

template<typename T>
void Buffer<T>::StoreData(std::vector<T> const& data)
{
    glBufferStorage((int)m_type, sizeof(data.data()), data.data(), m_dataPersistanceFlag);
}

