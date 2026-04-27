#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <array>

#include <Logger.hpp>

using namespace Ore;
std::array<glm::vec3, 8> Camera::GetFrustum() const
{
    //TODO ADD FUNCTION
    return {};
}

glm::mat4 const& Camera::GetViewProjMatrix() const
{
    return m_viewProjMat;
}

glm::vec3 Camera::GetPosition() const
{
    return { m_transform[0][3], m_transform[1][3], m_transform[2][3]};
}

void Camera::UpdateCamera()
{
    glm::mat4 projMatrix;
    switch(m_projectionType)
    {
    case ProjectionType::PERSPECTIVE:
        projMatrix = glm::perspective(Perspective.fov,Perspective.aspectRatio, Perspective.nearPlane, Perspective.farPlane);
        break;
    case ProjectionType::ORTHOGRAPHIC:
        projMatrix = glm::mat4({
            2.0f / Orthographic.right,  0.0f,                       0.0f,   0.0f,
            0.0f,                       -2.0f / Orthographic.top,   0.0f,   0.0f,
            -1.0f,                      1.0f,                       1.0f,   0.0f,
            0.0f,                       0.0f,                       0.0f,   1.0f,
        });
        break;
    }

    m_viewProjMat = projMatrix * m_viewMatrix;
}

void Camera::SetTransform(glm::mat4 const& transform)
{
    m_viewMatrix = glm::inverse(transform);
}

void Camera::LookAt(glm::vec3 const& target)
{
    glm::vec3 camPos = { m_transform[0][3], m_transform[1][3], m_transform[2][3]};
    m_viewMatrix = glm::lookAt(camPos, target, Perspective.up);
}
