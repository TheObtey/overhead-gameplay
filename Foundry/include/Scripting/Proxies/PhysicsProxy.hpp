#ifndef FOUNDRY_RAYCASTPROXY__H_
#define FOUNDRY_RAYCASTPROXY__H_

#include "Servers/PhysicsServer.h"

#include <Define.h>
#include <Event.hpp>
#include <optional>

namespace Physics
{
	struct RaycastHit 
	{
		glm::vec3 impactPos;
		glm::vec3 normal;
		float dist;
		NodeRigidBody::Proxy* node;
	};

	namespace HelperCallBack
	{
		class RaycastCB : public rp3d::RaycastCallback
		{
		public:
			std::vector<RaycastHit> hits;

			rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override
			{
				hits.push_back({
					rp3dToGlm(info.worldPoint),
					rp3dToGlm(info.worldNormal),
					info.hitFraction,
					static_cast<NodeRigidBody::Proxy*>(info.body->getUserData())
					});
				return 1.0;
			}
		};
	};

	std::optional<RaycastHit> RaycastClosest(const glm::vec3& origin, const glm::vec3& direction, float maxDist = 1000, uint16 mask = 0xFFFF);

	std::vector<RaycastHit> RaycastAll(const glm::vec3& origin, const glm::vec3& direction, float maxDist = 1000, uint16 mask = 0xFFFF);
};


struct PhysicsProxyBinding
{
	static void Bind(Binder& binder)
	{
		using rcHit = Physics::RaycastHit;
		auto physics = binder.GetOrCreateNamespace("physics");
		physics.new_usertype<rcHit>("raycastHit",
			"impactPos", &rcHit::impactPos,
			"normal", &rcHit::normal,
			"dist", &rcHit::dist,
			"node", &rcHit::node);

		physics.set_function("Raycast", Physics::RaycastClosest);
		physics.set_function("RaycastAll", Physics::RaycastAll);
		physics.set_function("SetGravity", &PhysicsServer::SetGravity);
		physics.set_function("GetGravity", &PhysicsServer::GetGravity); 
	}
};

REGISTER_PROXY(PhysicsProxyBinding, PhysicsProxy);

#endif // FOUNDRY_RAYCASTPROXY__H_