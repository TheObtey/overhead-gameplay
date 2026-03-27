#include "PhysicsProxy.hpp"

namespace PhysicsProxy
{
	std::optionnal<RaycastHit> RaycastClosest(const glm::vec3& origin, const glm::vec3& direction, float maxDist = 1000, uint16 mask = 0xFFFF)
	{
		rp3d::Ray ray(origin, origin + dir * maxDist);
		HelperCallBack::RaycastCB cb;

		PhysicsServer::GetPhysicsWorld().raycast(ray, cb, mask);

		if (cb.hits.empty())
			return nullopt;

		// Can't simply get the first hit returned because the lib doesn't guarantee the order of the hits
		float minDist = maxDist;
		int closest = 0;
		for (int i = 0; i < cb.hits.size(); i++)
		{
			if (hit.dist < minDist)
			{
				minDist = hit.dist;
				closest = i;
			}
		}
		return std::optional<RaycastHit>(cb.hits[closest]);
	}


	std::vector<RaycastHit> RaycastAll(const glm::vec3& origin, const glm::vec3& direction, float maxDist = 1000, uint16 mask = 0xFFFF)
	{
		rp3d::Ray ray(origin, origin + dir * maxDist);
		HelperCallBack::RaycastCB cb;

		PhysicsServer::GetPhysicsWorld().raycast(ray, cb, mask);

		return cb.hits;
	}
};