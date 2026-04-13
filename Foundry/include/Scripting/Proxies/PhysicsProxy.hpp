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

	inline auto RaycastClosest = sol::overload(
		[](const glm::vec3& origin, const glm::vec3& direction)
		{
			rp3d::Ray ray(glmToRp3d(origin), glmToRp3d(origin + direction * 100.0f));
			HelperCallBack::RaycastCB cb;

			PhysicsServer::GetPhysicsWorld().raycast(ray, &cb, 0);
			bool hasHit = !cb.hits.empty();
			int closest = 0;
			if (hasHit)
			{
				// Can't simply get the first hit returned because the lib doesn't guarantee the order of the hits
				float minDist = 100.0f;
				for (int i = 0; i < cb.hits.size(); i++)
				{
					if (cb.hits[i].dist < minDist)
					{
						minDist = cb.hits[i].dist;
						closest = i;
					}
				}
			}

			return hasHit ? std::optional<RaycastHit>(cb.hits[closest]) : std::nullopt;
		},
		[](const glm::vec3& origin, const glm::vec3& direction, float maxDist)
		{
			rp3d::Ray ray(glmToRp3d(origin), glmToRp3d(origin + direction * maxDist));
			HelperCallBack::RaycastCB cb;

			PhysicsServer::GetPhysicsWorld().raycast(ray, &cb, 0);

			bool hasHit = !cb.hits.empty();
			int closest = 0;
			if (hasHit)
			{
				// Can't simply get the first hit returned because the lib doesn't guarantee the order of the hits
				float minDist = maxDist;
				for (int i = 0; i < cb.hits.size(); i++)
				{
					if (cb.hits[i].dist < minDist)
					{
						minDist = cb.hits[i].dist;
						closest = i;
					}
				}
			}
			return hasHit ? std::optional<RaycastHit>(cb.hits[closest]) : std::nullopt;
		},
		[](const glm::vec3& origin, const glm::vec3& direction, float maxDist, uint16 mask)
		{
			rp3d::Ray ray(glmToRp3d(origin), glmToRp3d(origin + direction * maxDist));
			HelperCallBack::RaycastCB cb;

			PhysicsServer::GetPhysicsWorld().raycast(ray, &cb, mask);

			bool hasHit = !cb.hits.empty();
			int closest = 0;
			if (hasHit)
			{
				// Can't simply get the first hit returned because the lib doesn't guarantee the order of the hits
				float minDist = maxDist;
				for (int i = 0; i < cb.hits.size(); i++)
				{
					if (cb.hits[i].dist < minDist)
					{
						minDist = cb.hits[i].dist;
						closest = i;
					}
				}
			}
			return hasHit ? std::optional<RaycastHit>(cb.hits[closest]) : std::nullopt;
		}
	);

	inline auto RaycastAll = sol::overload(
		[](const glm::vec3& origin, const glm::vec3& direction)
		{
			rp3d::Ray ray(glmToRp3d(origin), glmToRp3d(origin + direction * 100.0f));
			HelperCallBack::RaycastCB cb;

			PhysicsServer::GetPhysicsWorld().raycast(ray, &cb, 0);

			return cb.hits;
		},
		[](const glm::vec3& origin, const glm::vec3& direction, float maxDist)
		{
			rp3d::Ray ray(glmToRp3d(origin), glmToRp3d(origin + direction * maxDist));
			HelperCallBack::RaycastCB cb;

			PhysicsServer::GetPhysicsWorld().raycast(ray, &cb, 0);

			return cb.hits;
		},
		[](const glm::vec3& origin, const glm::vec3& direction, float maxDist, uint16 mask)
		{
			rp3d::Ray ray(glmToRp3d(origin), glmToRp3d(origin + direction * maxDist));
			HelperCallBack::RaycastCB cb;

			PhysicsServer::GetPhysicsWorld().raycast(ray, &cb, mask);

			return cb.hits;
		}
	);
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

#endif // FOUNDRY_RAYCASTPROXY__H_