#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <array>

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
    glm::mat4 const projMatrix = glm::perspective(Perspective.fov,Perspective.aspectRatio, Perspective.nearPlane, Perspective.farPlane);
    m_viewProjMat = projMatrix * m_viewMatrix;
}

void Camera::SetTransform(glm::mat4 const& transform)
{
    m_viewMatrix = glm::scale(glm::inverse(transform), { -1.0,-1.0,-1.0 });
}

void Camera::LookAt(glm::vec3 const& target)
{
    glm::vec3 camPos = { m_transform[0][3], m_transform[1][3], m_transform[2][3]};
    m_viewMatrix = glm::lookAt(camPos, target, Perspective.up);
}
