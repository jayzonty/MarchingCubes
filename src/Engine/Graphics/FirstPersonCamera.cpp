#include "Engine/Graphics/FirstPersonCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Constructor
 */
FirstPersonCamera::FirstPersonCamera()
    : Camera()
{
}

/**
 * @brief Destructor
 */
FirstPersonCamera::~FirstPersonCamera()
{
}

/**
 * @brief Sets this camera's position
 * @param[in] position New position
 */
void FirstPersonCamera::SetPosition(const glm::vec3& position)
{
    m_position = position;
    UpdateVectors();
}

/**
 * @brief Sets the yaw angle of this camera
 * @param[in] yaw Yaw angle
 */
void FirstPersonCamera::SetYaw(const float& yaw)
{
    m_yaw = yaw;
    UpdateVectors();
}

/**
 * @brief Gets the yaw angle of this camera
 * @return Yaw angle
 */
float FirstPersonCamera::GetYaw()
{
    return m_yaw;
}

/**
 * @brief Sets the pitch angle of this camera
 * @param[in] pitch Pitch angle
 */
void FirstPersonCamera::SetPitch(const float& pitch)
{
    m_pitch = pitch;
    UpdateVectors();
}

/**
 * @brief Gets the pitch angle of this camera
 * @return Pitch angle
 */
float FirstPersonCamera::GetPitch()
{
    return m_pitch;
}

/**
 * @brief Gets the view matrix for this camera
 * @return View matrix
 */
glm::mat4 FirstPersonCamera::GetViewMatrix()
{
    return glm::lookAt(m_position, m_position + m_forward, m_globalUp);
}

/**
 * @brief Update vectors
 */
void FirstPersonCamera::UpdateVectors()
{
    m_forward.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_forward.y = glm::sin(glm::radians(m_pitch));
    m_forward.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));

    m_right = glm::normalize(glm::cross(m_forward, m_globalUp));
    m_up = glm::cross(m_right, m_forward);
}

