#include "Engine/Graphics/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Constructor
 */
Camera::Camera()
    : m_fov(90.0f)
    , m_aspectRatio(1.0f)
    , m_position(0.0f)
    , m_forward(0.0f, 0.0f, -1.0f)
    , m_right(1.0f, 0.0f, 0.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_globalUp(0.0f, 1.0f, 0.0f)
{
}

/**
 * @brief Destructor
 */
Camera::~Camera()
{
}

/**
 * @brief Sets the camera's field of view
 * @param[in] fov New field of view (in degrees)
 */
void Camera::SetFieldOfView(const float& fov)
{
    m_fov = fov;
}

/**
 * @brief Gets the camera's field of view
 * @return Field of view (in degrees)
 */
float Camera::GetFieldOfView()
{
    return m_fov;
}

/**
 * @brief Sets the camera's aspect ratio
 * @param[in] aspectRatio Aspect ratio
 */
void Camera::SetAspectRatio(const float& aspectRatio)
{
    m_aspectRatio = aspectRatio;
}

/**
 * Gets the camera's aspect ratio
 * @return Aspect ratio
 */
float Camera::GetAspectRatio()
{
    return m_aspectRatio;
}

/**
 * @brief Gets the camera's position
 * @return Camera position
 */
glm::vec3 Camera::GetPosition()
{
    return m_position;
}

/**
 * @brief Gets the camera's forward vector (world-space)
 * @return Camera's forward vector (world-space)
 */
glm::vec3 Camera::GetForwardVector()
{
    return m_forward;
}

/**
 * @brief Gets the camera's right vector (world-space)
 * @return Camera's right vector (world-space)
 */
glm::vec3 Camera::GetRightVector()
{
    return m_right;
}

/**
 * @brief Gets the camera's up vector (world-space)
 * @return Camera's up vector (world-space)
 */
glm::vec3 Camera::GetUpVector()
{
    return m_up;
}

/**
 * @brief Sets the global up vector
 * @param[in] globalUp Global up vector
 */
void Camera::SetGlobalUpVector(const glm::vec3& globalUp)
{
    m_globalUp = globalUp;
    UpdateVectors();
}

/**
 * @brief Gets the global up vector
 * @return Global up vector
 */
glm::vec3 Camera::GetGlobalUpVector()
{
    return m_globalUp;
}

/**
 * @brief Gets the projection matrix for this camera
 * @return Projection matrix
 */
glm::mat4 Camera::GetProjectionMatrix()
{
    // TODO: Make the near and far values adjustable
    return glm::perspective(glm::radians(m_fov), m_aspectRatio, 0.1f, 100.0f);
}

