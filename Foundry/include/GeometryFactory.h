#ifndef FOUNDRY_GEOMETRY_FACTORY_H__
#define FOUNDRY_GEOMETRY_FACTORY_H__

#include <Geometry.h>
#include <Debug.h>
#include <unordered_map>

struct GeoInfo {
	GeoInfo() = default;
	GeoInfo(std::vector<Vertex> vertices, std::vector<uint32> indices) {
		m_vertices = std::move(vertices);
		m_indices = std::move(indices);
	};
	std::vector<Vertex> m_vertices;
	std::vector<uint32> m_indices;
};

enum class PrimitivesType {
	CUBE,
	SPHERE,
	CYLINDER,
	CAPSULE
};

class GeometryFactory {
public:
	// return info of 1x1x1 cube, r = 0.5 sphere, r = 0.5 & height = 1 cylinder, r = 0.5 & height = 1 Capsule
	static const GeoInfo& GetGeometry(PrimitivesType type);

	static GeoInfo MakeCube(float width, float height, float depth);

	static GeoInfo CreateSphere(float radius, uint32 subdivisions);
	static GeoInfo CreateCylinder(float radius, float height, uint32 radialSegments, uint32 heightSegments);
	static GeoInfo CreateCapsule(float radius, float height, uint32 radialSegments, uint32 heightSegments, uint32 capSegments);

private:
	static std::vector<Vertex> CreateCubeVertices(float width, float height, float depth);
	static std::vector<uint32> CreateCubeIndices();
private:
	static inline std::unordered_map<PrimitivesType, GeoInfo> m_GeoInfo;
};

#endif //FOUNDRY_GEOMETRY_FACTORY__H_
