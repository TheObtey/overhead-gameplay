#ifndef RHI_IVERTEX_ARRAY_OBJECT__H_
#define RHI_IVERTEX_ARRAY_OBJECT__H_

#include "IObject.h"

class IVertexArrayObject : public IObject
{
public:
    virtual ~IVertexArrayObject() = 0;

    virtual void Bind() override = 0;
};    

inline IVertexArrayObject::~IVertexArrayObject() {}
#endif //!RHI_IVERTEX_ARRAY_OBJECT__H_
