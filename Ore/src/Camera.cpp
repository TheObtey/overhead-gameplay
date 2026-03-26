#include "Camera.h"
#include "Logger.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 const& position, glm::vec3 const& up, float yaw, float pitch, float roll, float fov)
{
    m_position = position;
    m_worldUp = up;
    m_forward = glm::vec3(0.0f);
    m_yaw = yaw;
    m_pitch = pitch;
    m_fov = fov;
    m_roll = roll;

    UpdateVectors();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetProjectionMatrix(ProjectionType type, uint16 screenWidth, uint16 screenHeight, float near, float far) const
{
    glm::mat4 projMatrix;

    switch(type)
    {
    case ProjectionType::PERSPECTIVE :
        projMatrix = glm::perspective(glm::radians(m_fov), (float)screenWidth / (float)screenHeight, near, far);
        break;
    case ProjectionType::ORTHOGRAPHIC : 
        projMatrix = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, near, far);
        break;
    }

    return projMatrix;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_forward, m_up);
}

void Camera::UpdateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_forward = glm::normalize(front);

    glm::vec3 right = glm::normalize(glm::cross(m_forward, m_worldUp));

    m_up = glm::normalize(glm::cross(right, m_forward));

    float rollRad = glm::radians(m_roll);
    glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), rollRad, m_forward);
    m_up = rollMatrix * glm::vec4(m_up, 0.0f);
}
