template <typename T>
Buffer<T>::Buffer(std::vector<T> const& data, uint32 id, BufferType type, bool isDataPersistant)
{
    m_id = id;
    m_type = type;    
    isDataPersistant ? m_dataPersistanceFlag = GL_MAP_PERSISTENT_BIT|GL_MAP_COHERENT_BIT|GL_MAP_WRITE_BIT : m_dataPersistanceFlag = GL_DYNAMIC_STORAGE_BIT;

    Bind();
    StoreData(data);
}

template<typename T>
Buffer<T>::Buffer(Buffer<T> const& other)
{
    m_id = other.m_id;
    m_type = other.m_type;
    m_dataPersistanceFlag = other.m_dataPersistanceFlag;
}

template <typename T>
Buffer<T>::~Buffer()
{
}

template <typename T>
void Buffer<T>::Bind()
{
    glBindBuffer(static_cast<uint32>(m_type), m_id);
}

template<typename T>
T* Buffer<T>::Map(uint32 offset, uint32 size)
{
    void* ptr = glMapBufferRange(static_cast<uint32>(m_type), offset, size, m_dataPersistanceFlag);
    return (T*)ptr;
}

template<typename T>
void Buffer<T>::Unmap()
{    
    glUnmapBuffer(static_cast<uint32>(m_type));
}

template<typename T>
void Buffer<T>::StoreData(std::vector<T> const& data)
{
    glBufferStorage(static_cast<uint32>(m_type), data.size() * sizeof(T), &data[0], m_dataPersistanceFlag);
}

