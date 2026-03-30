#ifndef CAMERA__H_
#define CAMERA__H_

namespace Camera
{
	struct Plane
	{
		glm::vec3 normal;
		float distance;
	};
	struct Frustum
	{
		Plane planes[6];
	};

	Frustum ExtractFrustum(const glm::mat4& vp)
	{
		Frustum f;
		auto row = [&](int i)
			{
				return glm::vec4(vp[0][i], vp[1][i], vp[2][i], vp[3][i]);
			};

		// Get rows (glm is column major)
		glm::vec4 r0 = row(0), r1 = row(1), r2 = row(2), r3 = row(3);

		auto make_plane = [](glm::vec4 p) -> Plane
			{
				float length = glm::length(glm::vec3(p));
				return { glm::vec3(p) / length, p.w / length };
			};
		f.planes[0] = make_plane(r3 + r0); // left
		f.planes[1] = make_plane(r3 - r0); // right
		f.planes[2] = make_plane(r3 + r1); // bottom
		f.planes[3] = make_plane(r3 - r1); // top
		f.planes[4] = make_plane(r3 + r2); // near
		f.planes[5] = make_plane(r3 - r2); // far

		return f;
	}

	bool IsPointInFrustum(const Frustum& f, glm::vec3 point)
	{
		for (const auto& plane : f.planes)
		{
			if (glm::dot(plane.normal, point) + plane.distance < 0.0f)
				return false;
		}
		return true;
	}

	bool IsSphereInFrustum(const Frustum& f, glm::vec3 center, float radius)
	{
		for (const auto& plane : f.planes)
		{
			if (glm::dot(plane.normal, center) + plane.distance < -radius)
				return false;
		}
		return true;
	}

	bool IsAABBInFrustum(const Frustum& f, glm::vec3 aabb_min, glm::vec3 aabb_max)
	{
		for (const auto& plane : f.planes)
		{
			glm::vec3 point =
			{
				plane.normal.x >= 0 ? aabb_max.x : aabb_min.x,
				plane.normal.y >= 0 ? aabb_max.y : aabb_min.y,
				plane.normal.z >= 0 ? aabb_max.z : aabb_min.z
			};

			if (glm::dot(plane.normal, point) + plane.distance < 0.0f)
				return false;
		}
		return true;
	}
}


#endif CAMERA__H_