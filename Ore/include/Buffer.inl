#include "Buffer.h" //TODO delete this line before push

#include <glad/glad.h>

template <typename T>
Buffer<T>::Buffer(std::vector<T> data, uint32 id, BufferType type)
{
    m_id = id;
    m_type = type;    
    m_data = data;
}

template <typename T>
void Buffer<T>::Bind()
{
    glBindBuffer((int)m_type, m_id);
}

template<typename T>
T* Buffer<T>::Map()
{
    void* ptr = glMapBuffer((int)m_type, m_id);
    return ptr;
}

template<typename T>
void Buffer<T>::Unmap()
{    
    glUnmapBuffer((int)m_type);
}

template<typename T>
void Buffer<T>::StoreData(std::vector<T> data)
{
    glBufferStorage((int)m_type, sizeof(data.data()), data.data(), GL_MAP_PERSISTENT_BIT|GL_MAP_COHERENT_BIT);
}

template<typename T>
void Buffer<T>::Load()
{
    Bind();
    StoreData(m_data);
}

template<typename T>
void Buffer<T>::Unload()
{
    glClearBufferData((int)m_type, sizeof(m_data.data), m_data.data());    
}
