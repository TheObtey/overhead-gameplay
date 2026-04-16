#ifndef ANIMATION_SERVER__H_
#define ANIMATION_SERVER__H_

#include "Server.hpp"
#include "AnimationStructs.h"

#include <Define.h>

class Animation;
namespace Ore { class Mesh; }

template <>
struct Command<class AnimationServer>
{
    enum class CmdType { UPDATE_FRAME, ADD_MESH, PRESENT } Type;
    Animation* pAnim = nullptr;
    Ore::Mesh* pMesh = nullptr;
};

class AnimationServer : public Server<AnimationServer>
{
public:
    using CommandType = Command<AnimationServer>::CmdType;

    static void UpdateFrame(Animation* pAnim, Ore::Mesh* pMesh);
    static void AddMesh(Ore::Mesh* pMesh);
private:
    static void UpdateFrameImediate(Animation* pAnim, Ore::Mesh* pMesh);
    static void AddMeshImmediate(Ore::Mesh* pMesh);
    static void PresentMeshs();
    static void BoneGlobalTransformHierarchy(Ore::Mesh* pMesh, uint32 ID, glm::mat4 const& parentMatrix);

    static glm::mat4 InterpolateFrames(AnimationChannel const& pChannel, float delta);

    void FlushCommandsImpl() override;
    void BuildTasksImpl(TaskGraph& graph) override {};
    void OnInitialize() override {}
};

#endif
