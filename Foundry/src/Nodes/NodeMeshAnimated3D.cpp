#include "Nodes/NodeMeshAnimated3D.h"
#include "Servers/AnimationServer.h"

NodeMeshAnimated3D::NodeMeshAnimated3D(std::string const& name) : Node3D::Node3D(name)
{}

void NodeMeshAnimated3D::OnUpdate(double delta)
{
	Node3D::OnUpdate(delta);
	if (m_isPlaying)
	{
		Animation& anim = *m_linkedAnimations[m_currentAnim];
		anim.currentTime += delta;
		if (anim.currentTime == anim.duration)
		{
			if (anim.isLooping == false)
			{
				StopCurrentAnim();
			}
			else
			{
				anim.currentTime = 0.0f;
				anim.isRestarting = true;
				AnimationServer::UpdateFrame(m_linkedAnimations[m_currentAnim].get(), m_mesh.get());
			}
		}
		else
			AnimationServer::UpdateFrame(m_linkedAnimations[m_currentAnim].get(), m_mesh.get());
	}
	AnimationServer::AddMesh(m_mesh.get());
}

void NodeMeshAnimated3D::Instanciate(Mesh const& mesh, Animation& anim)
{
	m_mesh = std::make_unique<Mesh>(mesh);
	Animation instanceAnim = {};
	instanceAnim.duration = anim.duration;
	instanceAnim.currentTime = 0.0f;
	instanceAnim.isLooping =false;
	instanceAnim.name = anim.name;
	for (uint8 i = 0; i < anim.animationTransform.size();++i)
	{
		instanceAnim.animationTransform.push_back(anim.animationTransform[i]);
		instanceAnim.animationTransform[i]->sceneNodeImpacted = i;
	}
	m_linkedAnimations[instanceAnim.name] = std::make_unique<Animation>(instanceAnim);
}

void NodeMeshAnimated3D::PlayAnimation(std::string const& name, bool isLooping)
{
	m_isPlaying = true;
	m_currentAnim = name;
	m_linkedAnimations[m_currentAnim]->isLooping = isLooping;
}

void NodeMeshAnimated3D::StopCurrentAnim()
{
	m_isPlaying = false;
	m_currentAnim = "";
}

ISerializable* NodeMeshAnimated3D::CreateInstance()
{
	return CreateNode<NodeMeshAnimated3D>("NodeMeshAnimated3D").release();
}

void NodeMeshAnimated3D::Deserialize(SerializedObject const& datas)
{
}

void NodeMeshAnimated3D::Serialize(SerializedObject& datas) const
{
}