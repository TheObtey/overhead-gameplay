#ifndef ORE_ANIMATED_PASS__H_
#define ORE_ANIMATED_PASS__H_

#include "Pass.h"
#include <vector>

namespace Ore
{
    class Mesh;
    class AnimatedPass final : public Pass
    {
    public:
        AnimatedPass(Program& shader);
        AnimatedPass(Program& shader, Camera* camera);
        ~AnimatedPass() = default;

        void Execute() override;
        void AddSkeletalMesh(Mesh const& mesh);

    private:
        std::vector<std::reference_wrapper<Mesh const>> m_skeletalMeshes;

    };
};
#endif
 
