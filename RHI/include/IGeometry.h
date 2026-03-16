#ifndef RHI_IGEOMETRY__H_
#define RHI_IGEOMETRY__H_

#include "Define.h"

class IGeometry 
{
public:
    virtual ~IGeometry() = 0;

};

inline IGeometry::~IGeometry() {};
#endif //!RHI_IGEOMETRY__H_
