#include "Nodes/NodeMeshAnimated3D.h"
#include "Servers/AnimationServer.h"
#include "Nodes/NodeViewport.h"

NodeMeshAnimated3D::NodeMeshAnimated3D(std::string const& name) : NodeVisual(name)
{
	m_isPlaying = false;
	m_currentAnim = "";
}

void NodeMeshAnimated3D::OnUpdate(double delta)
{
	NodeVisual::OnUpdate(delta);
	m_mesh->SetTransform(m_transform.GetMatrix());

	if (IsVisible() && m_pViewport)
		m_pViewport->AddSkeletalMesh(*this);

	if (m_isPlaying)
	{
		Animation& anim = *m_linkedAnimations[m_currentAnim];
		anim.currentTime += delta;

		if (anim.currentTime == anim.duration)
		{
			if (anim.isLooping == false)
			{
				StopCurrentAnim();
				return;
			}
			anim.currentTime = 0.0f;
			anim.isRestarting = true;
		}
		
		AnimationServer::UpdateFrame(m_linkedAnimations[m_currentAnim].get(), m_mesh.get());
	}
}

void NodeMeshAnimated3D::SetMesh(SceneMesh& mesh)
{

	sptr<Ore::Geometry> geo = std::make_shared<Ore::Geometry>(Ore::Geometry(mesh.vertices, mesh.indices));
	m_textures = mesh.meshTextures;
	m_mesh = std::make_unique<Ore::Mesh>(Ore::Mesh());
	m_mesh->SetGeometry(geo);
	m_mesh->SetTextures(m_textures);
	m_mesh->SetBonesOffsets(mesh.bonesOffest);
	glm::mat4 GlobalTransformation = glm::mat4(1.0f);

	std::vector<glm::mat4> bonesV = {};
	for (uint8 i = 0; i < mesh.bonesTransform.size();++i)
	{
		GlobalTransformation = GlobalTransformation * mesh.bonesTransform[i];
		bonesV.push_back(GlobalTransformation * mesh.bonesOffest[i]);
	}
	m_mesh->SetBones(bonesV);
}

void NodeMeshAnimated3D::SetAnimation(Animation& anim)
{
	Animation instanceAnim = {};
	instanceAnim.duration = anim.duration;
	instanceAnim.currentTime = 0.0f;
	instanceAnim.isLooping = false;
	instanceAnim.name = anim.name;
	for (uint8 i = 0; i < anim.animationTransform.size();++i)
	{
		instanceAnim.animationTransform.push_back(anim.animationTransform[i]);
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
	NodeVisual::Deserialize(datas);
}

void NodeMeshAnimated3D::AttachScriptDeserialize(uptr<LuaScriptInstance>& script)
{
	AttachScript<NodeMeshAnimated3D>(script, *this);
}

void NodeMeshAnimated3D::Serialize(SerializedObject& datas) const
{
	NodeVisual::Serialize(datas);
}
