#ifndef RHI_IGEOMETRY__H_
#define RHI_IGEOMETRY__H_

#include "Define.h"

#include <vector>

using GeometryID = uint32;
class IGeometry 
{
public:
    IGeometry();
    ~IGeometry();

    void AddPoints(std::vector<float> points);
    void AddIndices(std::vector<uint32> indices);

    GeometryID Load();
    void Unload(GeometryID id);

protected:
    GeometryID m_id;
};

#endif //!RHI_IGEOMETRY__H_
