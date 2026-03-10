#include "Buffer.h" 

#include <glad/glad.h>

template <typename T>
Buffer<T>::Buffer(std::vector<T> const& data, uint32 id, BufferType type, bool isDataPersistant)
{
    m_id = id;
    m_type = type;    
    StoreData(data, isDataPersistant);
}

template <typename T>
void Buffer<T>::Bind()
{
    glBindBuffer((int)m_type, m_id);
}

template<typename T>
T* Buffer<T>::Map(uint32 offset, uint32 size)
{
    void* ptr = glMapBufferRange((int)m_type, offset, size, GL_MAP_PERSISTENT_BIT|GL_MAP_COHERENT_BIT);
    return ptr;
}

template<typename T>
void Buffer<T>::Unmap()
{    
    glUnmapBuffer((int)m_type);
}

template<typename T>
void Buffer<T>::StoreData(std::vector<T> const& data, bool isPersistant)
{
    int flag = 0;
    isPersistant ? flag = GL_MAP_PERSISTENT_BIT : flag = GL_MAP_COHERENT_BIT;  
    glBufferStorage((int)m_type, sizeof(data.data()), data.data(), flag);
}

