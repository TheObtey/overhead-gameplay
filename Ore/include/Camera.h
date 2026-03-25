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
        glm::vec3 const& position = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 const& up = glm::vec3(0.0f, 0.0f, 0.0f),
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

    void SetPosition(glm::vec3 const& position)    { m_position = position;}

    void SetYaw(float yaw) {m_yaw = yaw; UpdateVectors(); };
    void SetPitch(float pitch) {m_pitch = pitch; UpdateVectors(); }
    void SetRoll(float roll) {m_roll = roll; UpdateVectors(); }
    void SetFov(float fov) {m_fov = fov; }

    float GetYaw() const {return m_yaw;}
    float GetPitch() const {return m_pitch;}
    float GetRoll() const {return m_roll;}
    
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
