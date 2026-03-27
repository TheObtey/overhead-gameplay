#ifndef FOUNDRY_RAYCASTPROXY__H_
#define FOUNDRY_RAYCASTPROXY__H_

#include "PhysicsServer.h"

#include <Define.h>
#include <Event.hpp>
#include <glm/vec3.hpp>
#include <functional>
#include <optional>


class Node3DProxy;

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
				static_cast<Node3DProxy*>(info.body->getUserData()) 
				});
			return 1.0;
		}
	};

	//class PhysicsContactEvents : public rp3d::EventListener
	//{
	//public:
	//	std::function<void(/* return type */)> func;

	//	virtual void onContact(const rp3d::CollisionCallback::CallbackData& data) override
	//	{
	//		func(data);
	//	};
	//};

	//class PhysicsTriggerEvents : public rp3d::EventListener
	//{
	//public:
	//	std::function<void(/* return type */)> func;

	//	virtual void onTrigger(const rp3d::OverlapCallback::CallbackData& data) override
	//	{
	//		func(data);
	//	};
	//};
	//class PhysicsEvents : public rp3d::EventListener
	//{
	//public:
	//	std::function<void(/* return type */)> collisionFunc;
	//	std::function<void(/* return type */)> triggerFunc;

	//	virtual void onContact(const rp3d::CollisionCallback::CallbackData& data) override
	//	{
	//		collisionFunc(data);
	//	};


	//	virtual void onTrigger(const rp3d::OverlapCallback::CallbackData& data) override
	//	{
	//		triggerFunc(data);
	//	};
	//};
};

namespace PhysicsProxy
{
	struct RaycastHit
	{
		glm::vec3 impactPos;
		glm::vec3 normal;
		float dist;
		Node3DProxy* node; 
	};

	//using cbData = rp3d::CollisionCallback::CallbackData;

	// func arg has to be of form : [](const cbData& data) { /* do stuff*/ };
	// func arg has to be of form : [](const rp3d::CollisionCallback::CallbackData& data) { /* do stuff*/ };
	//inline auto ContactEvent = sol::overload(
	//	[](std::function<void(/* return type */)> _func)
	//	{
	//		HelperCallBack::PhysicsContactEvents ev;
	//		ev.func = _func;
	//	};

	//inline auto OverlapEvent = sol::overload(
	//	[](std::function<void(/* return type */)> _func)
	//	{
	//		HelperCallBack::PhysicsTriggerEvents ev;
	//		ev.func = _func;

	//	};

	//inline auto AllEvents = sol::overload(
	//	[](std::function<void(/* return type */)> _func)
	//	{
	//		HelperCallBack::PhysicsTriggerEvents ev;
	//		ev.func = _func;

	//	};

	std::optionnal<RaycastHit> RaycastClosest(const glm::vec3& origin, const glm::vec3& direction, float maxDist = 1000, uint16 mask = 0xFFFF);

	//inline auto RaycastClosest = sol::overload(
	//	[](const glm::vec3& origin, const glm::vec3& direction, float maxDist = 1000, uint16 mask = 0xFFFF)
	//	{
	//		rp3d::Ray ray(origin, origin + dir * maxDist);
	//		HelperCallBack::RaycastCB cb;
	//		//HelperCallBack::PhysicsEvents ev;
	//		//ev.func = [](const cbData& data) { /* do stuff*/ };
	//		PhysicsServer::GetPhysicsWorld().raycast(ray, cb, mask);
	//		if (cb.hits.empty())
	//			return nullptr;
	//		// Can't simply get the first hit returned because the lib doesn't guarantee the order of the hits
	//		float minDist = maxDist;
	//		int closest = 0;
	//		for (int i = 0; i < cb.hits.size(); i++)
	//		{
	//			if (hit.dist < minDist)
	//			{
	//				minDist = hit.dist;
	//				closest = i;
	//			}
	//		}
	//		return cb.hits[closest];
	//	}
	//);

	std::vector<RaycastHit> RaycastAll(const glm::vec3& origin, const glm::vec3& direction, float maxDist = 1000, uint16 mask = 0xFFFF);

	//inline auto RaycastAll = sol::overload(
	//	[](const glm::vec3& origin, const glm::vec3& direction, float maxDist = 1000, uint16 mask = 0xFFFF)
	//	{
	//		rp3d::Ray ray(origin, origin + dir * maxDist);
	//		HelperCallBack::RaycastCB cb;
	//		PhysicsServer::GetPhysicsWorld().raycast(ray, cb, mask);
	//		return cb.hits;
	//	}
	//);



	struct PhysicsProxyBinding
	{
		static void Bind(Binder& binder)
		{
			auto physics = binder.GetOrCreateNamespace("physics");
			physics.new_usertype<RaycastHit>("raycastHit",
				"impactPos", &RaycastHit::impactPos,
				"normal", &RaycastHit::normal,
				"dist", &RaycastHit::dist,
				"node", &RaycastHit::node);

			physics.set_function("Raycast", PhysicsProxy::RaycastClosest);
			physics.set_function("RaycastAll", PhysicsProxy::RaycastAll);
			physics.set_function("SetGravity", &PhysicsServer::SetGravity);
			physics.set_function("GetGravity", &PhysicsServer::GetGravity);
		}
	};
};

REGISTER_PROXY(PhysicsProxyBinding, PhysicsProx);

#endif // FOUNDRY_RAYCASTPROXY__H_