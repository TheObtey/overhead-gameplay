#ifndef ORE_CAMERA__H_
#define ORE_CAMERA__H_

#include "Define.h"

#include <glm/glm.hpp>

enum class ProjectionType
{
    PERSPECTIVE,
    ORTHOGRAPHIC,
};

class Camera 
{
public:
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f),
        float yaw = 0.0f,
        float pitch = 0.0f,
        float roll = 0.0f,
        float fov = 45.0f
    );
    ~Camera();

    glm::mat4 GetProjectionMatrix(ProjectionType type, uint16 screenWidth, uint16 screenHeight, float near, float far) const;
    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetPosition() const {return m_position;}
    
    void UpdateVectors();

    void SetPosition(glm::vec3 position)    { m_position = position; UpdateVectors(); }

    void SetYaw(float yaw) {m_yaw = yaw; UpdateVectors(); };
    void SetPitch(float pitch) {m_pitch = pitch; UpdateVectors(); }
    void SetFov(float fov) {m_fov = fov; }
    
private:
    float m_yaw;
    float m_pitch;
    float m_roll;
    float m_fov;
    
    glm::vec3 m_position;
    glm::vec3 m_up;
    glm::vec3 m_worldUp;
    glm::vec3 m_forward;
    glm::vec3 m_right;
};
#endif
