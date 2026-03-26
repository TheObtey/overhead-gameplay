#ifndef RHI_IOBJECT__H_
#define RHI_IOBJECT__H_

#include "Define.h"

class IObject 
{
public:
    virtual ~IObject() = 0;

    virtual void Bind() = 0;
};

inline IObject::~IObject() {};
#endif //!RHI_IOBJECT__H_
