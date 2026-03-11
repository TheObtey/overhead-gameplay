#ifndef ORE_GEOMETRY_PASS__H_
#define ORE_GEOMETRY_PASS__H_

#include "Pass.h"
#include "Geometry.h"

class GeometryPass final : public Pass
{
public:
    GeometryPass(Shader& shader, std::vector<sptr<Geometry>> const& geometries, uint32 gBuffer);
    ~GeometryPass() override;

    virtual void Execute() override;
    
private:
    std::vector<sptr<Geometry>> m_geometries;
    uint32 m_gBuffer;
};
#endif
 
