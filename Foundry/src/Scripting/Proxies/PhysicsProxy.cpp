#include "Scripting/Proxies/PhysicsProxy.hpp"
#include "Registries/AutomaticRegisterProxy.hpp"

namespace Physics
{
	std::optional<RaycastHit> RaycastClosest(const glm::vec3& origin, const glm::vec3& direction, float maxDist, uint16 mask)
	{
		rp3d::Ray ray(glmToRp3d(origin), glmToRp3d(origin + direction * maxDist));
		HelperCallBack::RaycastCB cb;

		PhysicsServer::GetPhysicsWorld().raycast(ray, &cb, mask);

		if (cb.hits.empty())
			return std::nullopt;

		// Can't simply get the first hit returned because the lib doesn't guarantee the order of the hits
		float minDist = maxDist;
		int closest = 0;
		for (int i = 0; i < cb.hits.size(); i++)
		{
			if (cb.hits[i].dist < minDist)
			{
				minDist = cb.hits[i].dist;
				closest = i;
			}
		}
		return std::optional<RaycastHit>(cb.hits[closest]);
	}

	std::vector<RaycastHit> RaycastAll(const glm::vec3& origin, const glm::vec3& direction, float maxDist, uint16 mask)
	{
		rp3d::Ray ray(glmToRp3d(origin), glmToRp3d(origin + direction * maxDist));
		HelperCallBack::RaycastCB cb;

		PhysicsServer::GetPhysicsWorld().raycast(ray, &cb, mask);

		return cb.hits;
	}
};

REGISTER_PROXY(PhysicsProxyBinding, PhysicsProxy);