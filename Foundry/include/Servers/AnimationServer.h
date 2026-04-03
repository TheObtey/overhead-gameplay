#ifndef ANIMATION_SERVER__H_
#define ANIMATION_SERVER__H_

#include "Server.hpp"
#include "AnimationStructs.h"

#include <Passes/AnimatedPass.h>
#include <glm/glm.hpp>
#include <Define.h>

class Animation;
class Mesh;

template <>
struct Command<class AnimationServer>
{
    enum class CmdType { UPDATE_FRAME, ADD_MESH, PRESENT } Type;
    Animation* pAnim = nullptr;
    Mesh* pMesh = nullptr;
};

class AnimationServer : public Server<AnimationServer>
{
public:
    using CommandType = Command<AnimationServer>::CmdType;

    static void UpdateFrame(Animation* pAnim, Mesh* pMesh);
    static void AddMesh(Mesh* pMesh);

    static void InitAnimationPass(AnimatedPass* pPass) { bigtemp = pPass;}
private:
    static void UpdateFrameImediate(Animation* pAnim, Mesh* pMesh);
    static void AddMeshImmediate(Mesh* pMesh);
    static void PresentMeshs();
    static void BoneGlobalTransformHierarchy(Mesh* pMesh, uint32 ID, glm::mat4 const& parentMatrix);

    static glm::mat4 InterpolateFrames(AnimationChannel const& pChannel, float delta);

    void FlushCommandsImpl() override;
    void BuildTasksImpl(TaskGraph& graph) override {};
    void OnInitialize() override {}

    inline static AnimatedPass* bigtemp = nullptr;
};

#endif
