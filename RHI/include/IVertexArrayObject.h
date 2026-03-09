#ifndef RHI_IVERTEX_ARRAY_OBJECT__H_
#define RHI_IVERTEX_ARRAY_OBJECT__H_

#include "IObject.h"

class IVertexArrayObject : public IObject
{
public:
    virtual ~IVertexArrayObject() = 0;

    virtual void Bind() override = 0;
    virtual void Load() override = 0;
    virtual void Unload() override = 0;
};    

#endif //!RHI_IVERTEX_ARRAY_OBJECT__H_
