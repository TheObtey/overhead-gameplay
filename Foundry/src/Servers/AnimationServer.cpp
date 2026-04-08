#include "Servers/AnimationServer.h"
#include "AnimationStructs.h"
#include "Mesh.h"


#include <Define.h>
#include <MathUtils.h>
#include <Logger.hpp>


void AnimationServer::UpdateFrame(Animation* pAnim, Ore::Mesh* pMesh)
{
    Instance().m_commands.push({ CommandType::UPDATE_FRAME, pAnim, pMesh });
}
void AnimationServer::AddMesh(Ore::Mesh* pMesh)
{
    Instance().m_commands.push({ CommandType::ADD_MESH, nullptr, pMesh});
}

glm::mat4 AnimationServer::InterpolateFrames(AnimationChannel const& pChannel, float delta)
{
    //glm::vec3 newPos = Maths::Lerp(pChannel.positionKeys[pChannel.currentFrame].vec, pChannel.positionKeys[pChannel.currentFrame + 1].vec, delta);
    //glm::quat newRot = Maths::Slerp(pChannel.rotationKeys[pChannel.currentFrame].quaternion, pChannel.rotationKeys[pChannel.currentFrame + 1].quaternion, delta);
    //glm::vec3 newScale = Maths::Lerp(pChannel.scalingKeys[pChannel.currentFrame].vec, pChannel.scalingKeys[pChannel.currentFrame + 1].vec, delta);

    //glm::mat4 rotationMatrix = glm::mat4_cast(newRot);
    //return Maths::Translate(newPos) * rotationMatrix * Maths::Scale(newScale);
    return glm::mat4(1.0);
}

void AnimationServer::BoneGlobalTransformHierarchy(Ore::Mesh* pMesh,uint32 ID, glm::mat4 const& parentTransform)
{
    glm::mat4 GlobalTransformation = parentTransform * pMesh->GetBoneValue(ID);
    pMesh->SetBoneValue(ID, pMesh->GetBoneValue(ID) * pMesh->GetBoneOffset(ID));
    //if (ID+1 < pMesh->GetBonesTransform().size())
    //    BoneGlobalTransformHierarchy(pMesh, ID+1,GlobalTransformation);
}

void AnimationServer::UpdateFrameImediate(Animation* pAnim, Ore::Mesh* pMesh)
{
    if (pAnim->isRestarting)
    {
        for (uint32 i = 0; i < pAnim->animationTransform.size(); ++i)
        {
            pAnim->animationTransform[i]->currentFrame = 0;
        }
        pAnim->isRestarting = false;
    }
    float currentTime = pAnim->currentTime;
    std::vector<glm::mat4> toTransform = std::vector<glm::mat4>(pAnim->animationTransform.size() + 1);
    for (uint32 i = 0; i < pAnim->animationTransform.size();++i)
    {
        AnimationChannel& channel = *pAnim->animationTransform[i];
        if (channel.currentFrame == channel.frameCount-1)
        {
            glm::vec3 newPos = channel.positionKeys[channel.currentFrame].vec;
            glm::quat newRot = channel.rotationKeys[channel.currentFrame].quaternion;
            glm::vec3 newScale = channel.scalingKeys[channel.currentFrame].vec;
            glm::mat4 rotationMatrix = glm::mat4_cast(newRot);
            toTransform[i] = (Maths::Translate(newPos) * rotationMatrix * Maths::Scale(newScale));
            continue;
        }
        if (pAnim->currentTime >= channel.positionKeys[channel.currentFrame+1].time)
        {
            channel.currentFrame += 1;
            glm::vec3 newPos = channel.positionKeys[channel.currentFrame].vec;
            glm::quat newRot = channel.rotationKeys[channel.currentFrame].quaternion;
            glm::vec3 newScale = channel.scalingKeys[channel.currentFrame].vec;
            glm::mat4 rotationMatrix = glm::mat4_cast(newRot);
            pMesh->SetBoneValue(channel.sceneNodeImpacted, (Maths::Translate(newPos) * rotationMatrix * Maths::Scale(newScale)) * pMesh->GetBoneValue(channel.sceneNodeImpacted));
        }
        //float ratio = pAnim->currentTime / (channel.positionKeys[channel.currentFrame].time + channel.positionKeys[channel.currentFrame + 1].time);
        //pMesh->SetBoneValue(channel.sceneNodeImpacted, InterpolateFrames(channel, ratio) * pMesh->GetBoneOffset(channel.sceneNodeImpacted));
    }
    BoneGlobalTransformHierarchy(pMesh, 0, glm::mat4(1.0f));
}

void AnimationServer::AddMeshImmediate(Ore::Mesh* pMesh)
{
    // ->AddSkeletalMesh(*pMesh);
}

void AnimationServer::FlushCommandsImpl()
{
    while (m_commands.empty() == false)
    {
        auto& [type, pAnim,pMesh] = m_commands.front();
        switch (type)
        {
        case CommandType::UPDATE_FRAME:
            UpdateFrameImediate(pAnim,pMesh);
            break;
        case CommandType::ADD_MESH:
            AddMeshImmediate(pMesh);
            break;
        }
        m_commands.pop();
    }
}