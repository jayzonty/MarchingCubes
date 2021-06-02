#include "Engine/Graphics/OrbitCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Constructor
 */
OrbitCamera::OrbitCamera()
    : Camera()
    , m_pivotPoint(0.0f)
    , m_orbitDistance(5.0f)
    , m_yaw(0.0f)
    , m_pitch(0.0f)
{
}

/**
 * @brief Destructor
 */
OrbitCamera::~OrbitCamera()
{
}

/**
 * @brief Sets the position that this camera is orbiting around
 * @param[in] pivotPosition Orbit pivot position
 */
void OrbitCamera::SetPivotPoint(const glm::vec3& pivotPoint)
{
    m_pivotPoint = pivotPoint;
    UpdateVectors();
}

/**
 * @brief Gets the position that this camera is orbiting around
 * @return Orbit pivot position
 */
glm::vec3 OrbitCamera::GetPivotPoint()
{
    return m_pivotPoint;
}

/**
 * @brief Sets the camera's orbit distance
 * @param[in] orbit Distance Orbit distance
 */
void OrbitCamera::SetOrbitDistance(const float& orbitDistance)
{
    m_orbitDistance = orbitDistance;
    UpdateVectors();
}

/**
 * @brief Gets the camera's orbit distance
 * @return Camera's orbit distance
 */
float OrbitCamera::GetOrbitDistance()
{
    return m_orbitDistance;
}

/**
 * @brief Sets the camera's orbit yaw angle
 * @param[in] yaw Orbit yaw angle (in degrees)
 */
void OrbitCamera::SetOrbitYaw(const float& yaw)
{
    m_yaw = yaw;
    UpdateVectors();
}

/**
 * @brief Gets the camera's orbit yaw angle
 * @return Orbit yaw angle (in degrees)
 */
float OrbitCamera::GetOrbitYaw()
{
    return m_yaw;
}

/**
 * @brief Sets the camera's orbit pitch angle
 * @param[in] pitch Orbit pitch angle (in degrees)
 */
void OrbitCamera::SetOrbitPitch(const float& pitch)
{
    m_pitch = pitch;
    UpdateVectors();
}

/**
 * @brief Gets the camera's pitch
 * @return Orbit pitch angle (in degrees)
 */
float OrbitCamera::GetOrbitPitch()
{
    return m_pitch;
}

/**
 * @brief Gets the view matrix for this camera
 * @return View matrix
 */
glm::mat4 OrbitCamera::GetViewMatrix()
{
    return glm::lookAt(m_position, m_pivotPoint, m_globalUp);
}

/**
 * @brief Update vectors
 */
void OrbitCamera::UpdateVectors()
{
    m_forward.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_forward.y = glm::sin(glm::radians(m_pitch));
    m_forward.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_forward = -m_forward;

    m_position = m_pivotPoint - m_forward * m_orbitDistance;

    m_right = glm::normalize(glm::cross(m_forward, m_globalUp));
    m_up = glm::cross(m_right, m_forward);
}

