#ifndef RHI_IVERTEX_ARRAY_OBJECT__H_
#define RHI_IVERTEX_ARRAY_OBJECT__H_

#include "IObject.h"

namespace Ore
{
class IVertexArrayObject : public IObject
{
public:
    virtual void Bind() override = 0;
};
}
#endif //!RHI_IVERTEX_ARRAY_OBJECT__H_
