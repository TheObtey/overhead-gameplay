#ifndef ORE_OBJECT_FACTORY__H_
#define ORE_OBJECT_FACTORY__H_

class ObjectFactory 
{
public:
    static ObjectFactory& Instance();
    
private:
    ObjectFactory() = default;
    ~ObjectFactory() = default;

    static ObjectFactory m_pInstance;
};
#endif
