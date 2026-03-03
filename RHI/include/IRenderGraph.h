#ifndef RHI_IRENDER_GRAPH__H_
#define RHI_IRENDER_GRAPH__H_

#include "Define.h"
#include "IGeometry.h"
#include "IPass.h"

#include <vector>

class IRenderGraph 
{
public:
    IRenderGraph();
    ~IRenderGraph();

    virtual void AddGeometries(std::vector<uptr<IGeometry>> geometries) = 0;
    virtual void AddPass(uptr<IPass> pPass) = 0;

protected:
    std::vector<uptr<IGeometry>> m_geometries;
    std::vector<uptr<IPass>> m_passes; 

};
#endif //!RHI_IRENDER_GRAPH__H_
