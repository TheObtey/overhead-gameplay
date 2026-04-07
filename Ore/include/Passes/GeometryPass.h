#ifndef ORE_GEOMETRY_PASS__H_
#define ORE_GEOMETRY_PASS__H_

#include "Pass.h"
#include <vector>

class Mesh;
class GeometryPass final : public Pass
{
public:
    GeometryPass(Program& program);
    GeometryPass(Program& program, Camera* pCamera);
    ~GeometryPass() = default;

    void Execute() override;
    void AddMesh(Mesh const& mesh);
    
private:
    std::vector<std::reference_wrapper<Mesh const>> m_meshes;

};
#endif
 
