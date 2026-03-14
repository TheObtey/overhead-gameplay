#ifndef ORE_GEOMETRY_PASS__H_
#define ORE_GEOMETRY_PASS__H_

#include "Pass.h"
#include "Camera.h"

class Mesh;
class GeometryPass final : public Pass
{
public:
    GeometryPass(Shader const& shader, std::vector<Mesh> const& meshes, Camera const& camera);
    ~GeometryPass() override;

    virtual void Execute() override;
    
private:
    std::vector<sptr<Mesh>> m_meshes;
    sptr<Camera> m_pCamera;

};
#endif
 
