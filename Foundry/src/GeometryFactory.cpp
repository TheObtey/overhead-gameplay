#include "GeometryFactory.h"

#include <glm/common.hpp>
#include <glm/gtc/constants.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>

namespace
{
	using MidpointCache = std::unordered_map<uint64_t, uint32>;

	uint64_t BuildEdgeKey(uint32 a, uint32 b)
	{
		const uint32 lower = std::min(a, b);
		const uint32 higher = std::max(a, b);
		return (static_cast<uint64_t>(lower) << 32) | static_cast<uint64_t>(higher);
	}

	uint32 GetMidpointIndex(uint32 a, uint32 b, std::vector<glm::vec3>& positions, MidpointCache& cache)
	{
		const uint64_t key = BuildEdgeKey(a, b);
		auto it = cache.find(key);
		if (it != cache.end())
		{
			return it->second;
		}

		glm::vec3 midpoint = glm::normalize((positions[a] + positions[b]) * 0.5f);
		positions.push_back(midpoint);
		const uint32 index = static_cast<uint32>(positions.size() - 1);
		cache.emplace(key, index);
		return index;
	}
}

const GeoInfo& GeometryFactory::GetGeometry(PrimitivesType type)
{
	auto it = m_GeoInfo.find(type);
	if (it != m_GeoInfo.end())
	{
		return it->second;
	}

	GeoInfo geo;

	switch (type)
	{
	case PrimitivesType::CUBE:
		geo.m_vertices = CreateCubeVertices(1.0f, 1.0f, 1.0f);
		geo.m_indices = CreateCubeIndices();
		break;
	case PrimitivesType::SPHERE:
		geo = CreateSphere(0.5f, 3u);
		break;
	default:
		throw std::runtime_error("Not implemented");
	}

	auto [insertedIt, _] = m_GeoInfo.emplace(type, std::move(geo));
	return insertedIt->second;
}

void GeometryFactory::MakeCube(float width, float height, float depth)
{
	GeoInfo cubeInfo(CreateCubeVertices(width, height, depth), CreateCubeIndices());
	m_GeoInfo[PrimitivesType::CUBE] = std::move(cubeInfo);
}

void GeometryFactory::MakeSphere(float radius, uint32 subdivisions)
{
	m_GeoInfo[PrimitivesType::SPHERE] = CreateSphere(radius, subdivisions);
}

std::vector<Vertex> GeometryFactory::CreateCubeVertices(float width, float height, float depth)
{
	const float hx = width * 0.5f;
	const float hy = height * 0.5f;
	const float hz = depth * 0.5f;

	std::vector<Vertex> vertices;
	vertices.reserve(24);

	// +X
	vertices.push_back({ { hx, -hy, -hz }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } });
	vertices.push_back({ { hx,  hy, -hz }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } });
	vertices.push_back({ { hx,  hy,  hz }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } });
	vertices.push_back({ { hx, -hy,  hz }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } });

	// -X
	vertices.push_back({ { -hx, -hy,  hz }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } });
	vertices.push_back({ { -hx,  hy,  hz }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } });
	vertices.push_back({ { -hx,  hy, -hz }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } });
	vertices.push_back({ { -hx, -hy, -hz }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } });

	// +Y
	vertices.push_back({ { -hx,  hy, -hz }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } });
	vertices.push_back({ { -hx,  hy,  hz }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } });
	vertices.push_back({ {  hx,  hy,  hz }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } });
	vertices.push_back({ {  hx,  hy, -hz }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } });

	// -Y
	vertices.push_back({ { -hx, -hy,  hz }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } });
	vertices.push_back({ { -hx, -hy, -hz }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } });
	vertices.push_back({ {  hx, -hy, -hz }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } });
	vertices.push_back({ {  hx, -hy,  hz }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } });

	// +Z
	vertices.push_back({ { -hx, -hy,  hz }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } });
	vertices.push_back({ {  hx, -hy,  hz }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } });
	vertices.push_back({ {  hx,  hy,  hz }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } });
	vertices.push_back({ { -hx,  hy,  hz }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } });

	// -Z
	vertices.push_back({ {  hx, -hy, -hz }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } });
	vertices.push_back({ { -hx, -hy, -hz }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } });
	vertices.push_back({ { -hx,  hy, -hz }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } });
	vertices.push_back({ {  hx,  hy, -hz }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } });

	return vertices;
}

std::vector<uint32> GeometryFactory::CreateCubeIndices()
{
	std::vector<uint32> indices;
	indices.reserve(36);

	for (uint32 face = 0; face < 6; ++face)
	{
		const uint32 base = face * 4;
		indices.push_back(base + 0);
		indices.push_back(base + 1);
		indices.push_back(base + 2);
		indices.push_back(base + 0);
		indices.push_back(base + 2);
		indices.push_back(base + 3);
	}

	return indices;
}

GeoInfo GeometryFactory::CreateSphere(float radius, uint32 subdivisions)
{
	const float phi = (1.0f + std::sqrt(5.0f)) * 0.5f;

	std::vector<glm::vec3> positions = {
		{ -1.0f,  phi, 0.0f }, 
		{  1.0f,  phi, 0.0f }, 
		{ -1.0f, -phi, 0.0f }, 
		{  1.0f, -phi, 0.0f },

		{  0.0f, -1.0f,  phi }, 
		{  0.0f,  1.0f,  phi }, 
		{  0.0f, -1.0f, -phi }, 
		{  0.0f,  1.0f, -phi },

		{  phi, 0.0f, -1.0f }, 
		{  phi, 0.0f,  1.0f }, 
		{ -phi, 0.0f, -1.0f }, 
		{ -phi, 0.0f,  1.0f }
	};

	for (glm::vec3& pos : positions)
	{
		pos = glm::normalize(pos);
	}

	std::vector<uint32> indices = {
		 0, 11,  5,  0,  5,  1,  0,  1,  7,  0,  7, 10,  0, 10, 11,
		 1,  5,  9,  5, 11,  4, 11, 10,  2, 10,  7,  6,  7,  1,  8,
		 3,  9,  4,  3,  4,  2,  3,  2,  6,  3,  6,  8,  3,  8,  9,
		 4,  9,  5,  2,  4, 11,  6,  2, 10,  8,  6,  7,  9,  8,  1
	};

	subdivisions = std::min(subdivisions, 6u);
	for (uint32 level = 0; level < subdivisions; ++level)
	{
		MidpointCache midpointCache;
		midpointCache.reserve(indices.size());
		std::vector<uint32> newIndices;
		newIndices.reserve(indices.size() * 4);

		for (size_t idx = 0; idx < indices.size(); idx += 3)
		{
			const uint32 v0 = indices[idx + 0];
			const uint32 v1 = indices[idx + 1];
			const uint32 v2 = indices[idx + 2];

			const uint32 a = GetMidpointIndex(v0, v1, positions, midpointCache);
			const uint32 b = GetMidpointIndex(v1, v2, positions, midpointCache);
			const uint32 c = GetMidpointIndex(v2, v0, positions, midpointCache);

			newIndices.push_back(v0);
			newIndices.push_back(a);
			newIndices.push_back(c);

			newIndices.push_back(v1);
			newIndices.push_back(b);
			newIndices.push_back(a);

			newIndices.push_back(v2);
			newIndices.push_back(c);
			newIndices.push_back(b);

			newIndices.push_back(a);
			newIndices.push_back(b);
			newIndices.push_back(c);
		}

		indices = std::move(newIndices);
	}

	GeoInfo geo;
	geo.m_vertices.reserve(positions.size());
	geo.m_indices = std::move(indices);

	for (glm::vec3 pos : positions)
	{
		glm::vec3 normal = glm::normalize(pos);
		glm::vec3 scaledPos = normal * radius;

		const float u = std::atan2(normal.z, normal.x) / glm::two_pi<float>() + 0.5f;
		const float v = 0.5f - std::asin(std::clamp(normal.y, -1.0f, 1.0f)) / glm::pi<float>();

		geo.m_vertices.push_back({ scaledPos, normal, { u, v } });
	}

	return geo;
}