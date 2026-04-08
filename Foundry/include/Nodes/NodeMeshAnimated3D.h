#ifndef FOUNDRY_NODE_MESHANIMATED3D__H_
#define FOUNDRY_NODE_MESHANIMATED3D__H_

#include "Define.h"
#include "Nodes/NodeVisual.h"
#include "AnimationStructs.h"
#include "AssetLoading/AssetsStructs.h"

#include <map>
#include <Mesh.h>

class NodeMeshAnimated3D : public NodeVisual
{
public:
	//class Proxy
	explicit NodeMeshAnimated3D(std::string const& name);
	~NodeMeshAnimated3D() override = default;

	virtual void OnUpdate(double delta) override;
	
	void SetMesh(SceneMesh& mesh);
	void SetAnimation(Animation& anim);

	void PlayAnimation(std::string const& name, bool isLooping = false);
	void StopCurrentAnim();

	static ISerializable* CreateInstance();
	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;

protected:
	uptr<Ore::Mesh> m_mesh;
	std::map<std::string, uptr<Animation>> m_linkedAnimations;
	std::vector<sptr<Ore::Texture>> m_textures;
	std::string m_currentAnim;
	bool m_isPlaying;

	friend class NodeViewport;
};

REGISTER_ISERIALIZABLE(NodeMeshAnimated3D, NodeMeshAnimated3D::CreateInstance);

#endif

