#ifndef RHI_IGEOMETRY__H_
#define RHI_IGEOMETRY__H_

#include "Define.h"

#include <vector>

using GeometryID = uint32;
class IGeometry 
{
public:
    virtual ~IGeometry() = 0;

    virtual void AddPoints(std::vector<float> const& points) = 0;
    virtual void AddIndices(std::vector<uint32> const& indices) = 0;

    GeometryID Load();
    void Unload();

protected:
    GeometryID m_id;
};

#endif //!RHI_IGEOMETRY__H_
