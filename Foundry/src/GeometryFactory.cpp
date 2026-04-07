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
	case PrimitivesType::CYLINDER:
		geo = CreateCylinder(0.5f, 1.0f, 32u, 1u);
		break;
	case PrimitivesType::CAPSULE:
		geo = CreateCapsule(0.5f, 1.0f, 32u, 1u, 4u);
		break;
	default:
		throw std::runtime_error("Not implemented");
	}

	auto [insertedIt, _] = m_GeoInfo.emplace(type, std::move(geo));
	return insertedIt->second;
}

GeoInfo GeometryFactory::MakeCube(float width, float height, float depth)
{
	GeoInfo cubeInfo(CreateCubeVertices(width, height, depth), CreateCubeIndices());
	m_GeoInfo[PrimitivesType::CUBE] = std::move(cubeInfo);
	return m_GeoInfo[PrimitivesType::CUBE];
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
		{ -1.0f,  phi, 0.0f }, {  1.0f,  phi, 0.0f }, { -1.0f, -phi, 0.0f }, {  1.0f, -phi, 0.0f },
		{  0.0f, -1.0f,  phi }, {  0.0f,  1.0f,  phi }, {  0.0f, -1.0f, -phi }, {  0.0f,  1.0f, -phi },
		{  phi, 0.0f, -1.0f }, {  phi, 0.0f,  1.0f }, { -phi, 0.0f, -1.0f }, { -phi, 0.0f,  1.0f }
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

// Height Segement = 1 => 2 rings of vertices (top and bottom) + 1 center vertex for each cap
GeoInfo GeometryFactory::CreateCylinder(float radius, float height, uint32 radialSegments, uint32 heightSegments)
{
	radialSegments = std::max(radialSegments, 3u);
	heightSegments = std::max(heightSegments, 1u);

	const float halfHeight = height * 0.5f;
	const uint32 ringStride = radialSegments + 1u;
	const float invRadial = 1.0f / static_cast<float>(radialSegments);
	const float invHeight = 1.0f / static_cast<float>(heightSegments);

	std::vector<Vertex> vertices;
	vertices.reserve((heightSegments + 1u) * ringStride + (radialSegments + 1u) * 2u + 2u);

	std::vector<uint32> indices;
	indices.reserve(radialSegments * heightSegments * 6u + radialSegments * 6u);

	// Mantle
	for (uint32 ySegment = 0; ySegment <= heightSegments; ++ySegment)
	{
		const float v = static_cast<float>(ySegment) * invHeight;
		const float y = -halfHeight + v * height;

		for (uint32 r = 0; r <= radialSegments; ++r)
		{
			const float u = static_cast<float>(r) * invRadial;
			const float angle = u * glm::two_pi<float>();
			const float cosTheta = std::cos(angle);
			const float sinTheta = std::sin(angle);

			glm::vec3 normal(cosTheta, 0.0f, sinTheta);
			glm::vec3 position(radius * cosTheta, y, radius * sinTheta);
			vertices.push_back({ position, normal, { u, 1.0f - v } });
		}
	}

	for (uint32 ySegment = 0; ySegment < heightSegments; ++ySegment)
	{
		for (uint32 r = 0; r < radialSegments; ++r)
		{
			const uint32 current = ySegment * ringStride + r;
			const uint32 next = current + ringStride;

			indices.push_back(current);
			indices.push_back(next);
			indices.push_back(current + 1u);

			indices.push_back(current + 1u);
			indices.push_back(next);
			indices.push_back(next + 1u);
		}
	}

	// Top cap
	const uint32 topCenterIndex = static_cast<uint32>(vertices.size());
	vertices.push_back({ { 0.0f, halfHeight, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.5f, 0.5f } });

	const uint32 topRingStart = static_cast<uint32>(vertices.size());
	for (uint32 r = 0; r <= radialSegments; ++r)
	{
		const float u = static_cast<float>(r) * invRadial;
		const float angle = u * glm::two_pi<float>();
		const float cosTheta = std::cos(angle);
		const float sinTheta = std::sin(angle);

		glm::vec3 position(radius * cosTheta, halfHeight, radius * sinTheta);
		glm::vec2 uv(cosTheta * 0.5f + 0.5f, sinTheta * 0.5f + 0.5f);
		vertices.push_back({ position, { 0.0f, 1.0f, 0.0f }, uv });
	}

	for (uint32 r = 0; r < radialSegments; ++r)
	{
		const uint32 curr = topRingStart + r;
		const uint32 next = topRingStart + r + 1u;
		indices.push_back(topCenterIndex);
		indices.push_back(curr);
		indices.push_back(next);
	}

	// Bottom cap
	const uint32 bottomCenterIndex = static_cast<uint32>(vertices.size());
	vertices.push_back({ { 0.0f, -halfHeight, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.5f, 0.5f } });

	const uint32 bottomRingStart = static_cast<uint32>(vertices.size());
	for (uint32 r = 0; r <= radialSegments; ++r)
	{
		const float u = static_cast<float>(r) * invRadial;
		const float angle = u * glm::two_pi<float>();
		const float cosTheta = std::cos(angle);
		const float sinTheta = std::sin(angle);

		glm::vec3 position(radius * cosTheta, -halfHeight, radius * sinTheta);
		glm::vec2 uv(cosTheta * 0.5f + 0.5f, sinTheta * 0.5f + 0.5f);
		vertices.push_back({ position, { 0.0f, -1.0f, 0.0f }, uv });
	}

	for (uint32 r = 0; r < radialSegments; ++r)
	{
		const uint32 curr = bottomRingStart + r;
		const uint32 next = bottomRingStart + r + 1u;
		indices.push_back(bottomCenterIndex);
		indices.push_back(next);
		indices.push_back(curr);
	}

	GeoInfo geo;
	geo.m_vertices = std::move(vertices);
	geo.m_indices = std::move(indices);
	return geo;
}


// Height Segement = 1 => 2 rings of vertices (top and bottom) + 1 center vertex for each cap
GeoInfo GeometryFactory::CreateCapsule(float radius, float height, uint32 radialSegments, uint32 heightSegments, uint32 capSegments)
{
	radialSegments = std::max(radialSegments, 3u);
	heightSegments = std::max(heightSegments, 1u);
	capSegments = std::max(capSegments, 1u);

	const float halfCylinderHeight = height * 0.5f;
	const float capsuleHalfHeight = halfCylinderHeight + radius;
	const float totalHeight = capsuleHalfHeight * 2.0f;
	const float invRadial = 1.0f / static_cast<float>(radialSegments);
	const uint32 ringStride = radialSegments + 1u;

	std::vector<Vertex> vertices;
	vertices.reserve((heightSegments + 1u + (capSegments - 1u) * 2u) * ringStride + 2u);

	std::vector<uint32> indices;
	indices.reserve(radialSegments * (heightSegments + (capSegments - 1u) * 2u) * 6u + radialSegments * 6u);

	std::vector<uint32> cylinderRings;
	cylinderRings.reserve(heightSegments + 1u);

	auto computeV = [&](float y)
		{
			const float normalized = (y + capsuleHalfHeight) / totalHeight;
			return 1.0f - normalized;
		};

	// Cylinder rings
	for (uint32 ySegment = 0; ySegment <= heightSegments; ++ySegment)
	{
		const float t = static_cast<float>(ySegment) / static_cast<float>(heightSegments);
		const float y = -halfCylinderHeight + t * height;

		const uint32 ringStart = static_cast<uint32>(vertices.size());
		cylinderRings.push_back(ringStart);

		for (uint32 r = 0; r <= radialSegments; ++r)
		{
			const float u = static_cast<float>(r) * invRadial;
			const float angle = u * glm::two_pi<float>();
			const float cosTheta = std::cos(angle);
			const float sinTheta = std::sin(angle);

			glm::vec3 normal(cosTheta, 0.0f, sinTheta);
			glm::vec3 position(radius * cosTheta, y, radius * sinTheta);
			vertices.push_back({ position, normal, { u, computeV(position.y) } });
		}
	}

	// Top cap rings
	std::vector<uint32> topCapRings;
	topCapRings.reserve(capSegments);
	for (uint32 stack = 1u; stack < capSegments; ++stack)
	{
		const float t = static_cast<float>(stack) / static_cast<float>(capSegments);
		const float sinTheta = std::sin(t * glm::half_pi<float>());
		const float cosTheta = std::cos(t * glm::half_pi<float>());

		const uint32 ringStart = static_cast<uint32>(vertices.size());
		topCapRings.push_back(ringStart);

		for (uint32 r = 0; r <= radialSegments; ++r)
		{
			const float u = static_cast<float>(r) * invRadial;
			const float angle = u * glm::two_pi<float>();
			const float cosPhi = std::cos(angle);
			const float sinPhi = std::sin(angle);

			glm::vec3 normal = glm::normalize(glm::vec3(cosPhi * cosTheta, sinTheta, sinPhi * cosTheta));
			glm::vec3 position(radius * normal.x, halfCylinderHeight + radius * normal.y, radius * normal.z);
			vertices.push_back({ position, normal, { u, computeV(position.y) } });
		}
	}

	const uint32 topPoleIndex = static_cast<uint32>(vertices.size());
	vertices.push_back({ { 0.0f, halfCylinderHeight + radius, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.5f, computeV(halfCylinderHeight + radius) } });

	// Bottom cap rings
	std::vector<uint32> bottomCapRings;
	bottomCapRings.reserve(capSegments);
	for (uint32 stack = 1u; stack < capSegments; ++stack)
	{
		const float t = static_cast<float>(stack) / static_cast<float>(capSegments);
		const float sinTheta = std::sin(t * glm::half_pi<float>());
		const float cosTheta = std::cos(t * glm::half_pi<float>());

		const uint32 ringStart = static_cast<uint32>(vertices.size());
		bottomCapRings.push_back(ringStart);

		for (uint32 r = 0; r <= radialSegments; ++r)
		{
			const float u = static_cast<float>(r) * invRadial;
			const float angle = u * glm::two_pi<float>();
			const float cosPhi = std::cos(angle);
			const float sinPhi = std::sin(angle);

			glm::vec3 normal = glm::normalize(glm::vec3(cosPhi * cosTheta, -sinTheta, sinPhi * cosTheta));
			glm::vec3 position(radius * normal.x, -halfCylinderHeight + radius * normal.y, radius * normal.z);
			vertices.push_back({ position, normal, { u, computeV(position.y) } });
		}
	}

	const uint32 bottomPoleIndex = static_cast<uint32>(vertices.size());
	vertices.push_back({ { 0.0f, -(halfCylinderHeight + radius), 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.5f, computeV(-(halfCylinderHeight + radius)) } });

	auto appendStrip = [&](uint32 lowerStart, uint32 upperStart)
		{
			for (uint32 r = 0; r < radialSegments; ++r)
			{
				indices.push_back(lowerStart + r);
				indices.push_back(upperStart + r);
				indices.push_back(lowerStart + r + 1u);

				indices.push_back(lowerStart + r + 1u);
				indices.push_back(upperStart + r);
				indices.push_back(upperStart + r + 1u);
			}
		};

	for (uint32 segment = 0; segment < heightSegments; ++segment)
	{
		appendStrip(cylinderRings[segment], cylinderRings[segment + 1u]);
	}

	uint32 previousRing = cylinderRings.back();
	for (uint32 ringStart : topCapRings)
	{
		appendStrip(previousRing, ringStart);
		previousRing = ringStart;
	}

	const uint32 topFanRing = previousRing;
	for (uint32 r = 0; r < radialSegments; ++r)
	{
		indices.push_back(topFanRing + r);
		indices.push_back(topFanRing + r + 1u);
		indices.push_back(topPoleIndex);
	}

	uint32 upperRing = cylinderRings.front();
	for (uint32 ringStart : bottomCapRings)
	{
		appendStrip(ringStart, upperRing);
		upperRing = ringStart;
	}

	const uint32 bottomFanRing = bottomCapRings.empty() ? cylinderRings.front() : bottomCapRings.back();
	for (uint32 r = 0; r < radialSegments; ++r)
	{
		indices.push_back(bottomPoleIndex);
		indices.push_back(bottomFanRing + r + 1u);
		indices.push_back(bottomFanRing + r);
	}

	GeoInfo geo;
	geo.m_vertices = std::move(vertices);
	geo.m_indices = std::move(indices);
	return geo;
}