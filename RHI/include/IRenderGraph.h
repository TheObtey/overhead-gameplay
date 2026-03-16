#ifndef RHI_IRENDER_GRAPH__H_
#define RHI_IRENDER_GRAPH__H_

#include "Define.h"
#include "IGeometry.h"
#include "IPass.h"

class IRenderGraph 
{
public:
    virtual ~IRenderGraph() = 0;

    virtual void Execute() = 0;

protected:
    virtual void CreateGBuffer(uint32 screenWidth, uint32 screenHeight) = 0;
    

};

inline IRenderGraph::~IRenderGraph() {};
#endif //!RHI_IRENDER_GRAPH__H_
