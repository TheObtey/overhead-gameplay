#ifndef FOUNDRY_GEOMETRY_FACTORY_H__
#define FOUNDRY_GEOMETRY_FACTORY_H__

#include <Geometry.h>
#include <Debug.h>
#include <unordered_map>

/*
* struct Vertex inside Geometry.h
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};
*/
struct GeoInfo {
	GeoInfo() = default;
	GeoInfo(std::vector<Vertex> vertices, std::vector<uint32> indices){
		m_vertices = vertices;
		m_indices = indices;
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
	static const GeoInfo& GetGeometry(PrimitivesType type); // idealement retourner un Geometry directement et enlever la liaison dans le main 

	static void MakeCube(float width,float height,float depth);

	static void MakeSphere(float radius, uint32 subdivisions = 3u);

	//void MakeCylinder(float r, float height);
	//void MakeCapsule(float r, float height);
private:
	static std::vector<Vertex> CreateCubeVertices(float width, float height, float depth);
	static std::vector<uint32> CreateCubeIndices();
	static GeoInfo CreateSphere(float radius, uint32 subdivisions);


	//std::vector<Vertex> CreateCylinderVertices(float r, float height);
	//std::vector<uint32> CreateCylinderIndices();

	//std::vector<Vertex> CreateCapsuleVertices(float r, float height);
	//std::vector<uint32> CreateCapsuleIndices();
private:
	static inline std::unordered_map<PrimitivesType, GeoInfo> m_GeoInfo;
};

#endif //FOUNDRY_GEOMETRY_FACTORY__H_
