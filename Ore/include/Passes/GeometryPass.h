#ifndef ORE_GEOMETRY_PASS__H_
#define ORE_GEOMETRY_PASS__H_

#include "Pass.h"

class Mesh;
class GeometryPass final : public Pass
{
public:
    GeometryPass(Program const& shader, std::vector<Mesh*> const& meshes, sptr<Camera> camera);
    ~GeometryPass() override;

    void SetMeshes(std::vector<Mesh*> const& meshes) {m_meshes = meshes;}
    void Execute() override;
    
private:
    
    std::vector<Mesh*> m_meshes;

};
#endif
 
