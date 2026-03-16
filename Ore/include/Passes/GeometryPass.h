#ifndef ORE_GEOMETRY_PASS__H_
#define ORE_GEOMETRY_PASS__H_

#include "Pass.h"

class Mesh;
class GeometryPass final : public Pass
{
public:
    GeometryPass(Shader const& shader, std::vector<Mesh> const& meshes, Camera const& camera);
    ~GeometryPass() override;

    void SetMeshes(std::vector<Mesh> meshes);
    void Execute() override;
    
private:
    std::vector<sptr<Mesh>> m_meshes;

};
#endif
 
