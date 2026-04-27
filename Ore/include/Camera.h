#ifndef ORE_CAMERA__H_
#define ORE_CAMERA__H_

#include "Define.h"
#include "ICamera.h"
#include <glm/glm.hpp>
#include <array>

namespace Ore
{
enum class ProjectionType : uint8
{
    PERSPECTIVE,
    ORTHOGRAPHIC,
};

class Camera : public ICamera
{
public:
    struct PerspectiveSettings
    {
        float fov = 90;
        float aspectRatio = 1;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;
        glm::vec3 up = {0.0f, 1.0f, 0.0f};
    };

    struct OrthographicSettings
    {
        float left = 0.0f;
        float bottom = 0.0f;
        float right = 1920.0f;
        float top = 1080.0f;
        float nearPlane = -1.0f;
        float farPlane = 1.0f;
    };

    Camera() = default;
    Camera(PerspectiveSettings const settings) : Perspective(settings) {}
    Camera(OrthographicSettings const settings) : Orthographic(settings), m_projectionType(ProjectionType::ORTHOGRAPHIC) {}
    ~Camera() = default;

    std::array<glm::vec3, 8> GetFrustum() const;
    glm::mat4 const& GetViewProjMatrix() const;
    glm::vec3 GetPosition() const;

    void LookAt(glm::vec3 const& target);
    void UpdateCamera();

    void SetTransform(glm::mat4 const& transform);
    PerspectiveSettings Perspective = {};
    OrthographicSettings Orthographic = {};

private:
    glm::mat4 m_transform {1.0f};
    glm::mat4 m_viewMatrix {1.0f};
    glm::mat4 m_viewProjMat {1.0f};

    ProjectionType m_projectionType = ProjectionType::PERSPECTIVE;
};
}
#endif
