#include "Engine/Graphics/OrbitCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Constructor
 */
OrbitCamera::OrbitCamera()
	: m_fov(90.0f)
	, m_aspectRatio(1.0f)
	, m_position(0.0f)
    , m_pivotPosition(0.0f)
    , m_orbitDistance(5.0f)
	, m_yaw(0.0f)
	, m_pitch(0.0f)
	, m_forward(0.0f, 0.0f, -1.0f)
	, m_right(1.0f, 0.0f, 0.0f)
	, m_up(0.0f, 1.0f, 0.0f)
	, m_worldUp(0.0f, 1.0f, 0.0f)
{
	UpdateVectors();
}

/**
 * @brief Destructor
 */
OrbitCamera::~OrbitCamera()
{
}

/**
 * @brief Sets the camera's field of view
 * @param[in] fov New field of view (in degrees)
 */
void OrbitCamera::SetFieldOfView(const float& fov)
{
	m_fov = fov;
}

/**
 * @brief Gets the camera's field of view
 * @return Field of view (in degrees)
 */
float OrbitCamera::GetFieldOfView()
{
	return m_fov;
}

/**
 * @brief Sets the camera's aspect ratio
 * @param[in] aspectRatio Aspect ratio
 */
void OrbitCamera::SetAspectRatio(const float& aspectRatio)
{
	m_aspectRatio = aspectRatio;
}

/**
 * Gets the camera's aspect ratio
 * @return Aspect ratio
 */
float OrbitCamera::GetAspectRatio()
{
	return m_aspectRatio;
}

/**
 * @brief Sets the position that this camera is orbiting around
 * @param[in] pivotPosition Orbit pivot position
 */
void OrbitCamera::SetPivotPosition(const glm::vec3& pivotPosition)
{
    m_pivotPosition = pivotPosition;
    UpdateVectors();
}

/**
 * @brief Gets the position that this camera is orbiting around
 * @return Orbit pivot position
 */
glm::vec3 OrbitCamera::GetPivotPosition()
{
    return m_pivotPosition;
}

/**
 * @brief Gets the camera's position
 * @return Camera position
 */
glm::vec3 OrbitCamera::GetPosition()
{
	return m_position;
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
 * @brief Gets the camera's forward vector (world-space)
 * @return Camera's forward vector (world-space)
 */
glm::vec3 OrbitCamera::GetForwardVector()
{
	return m_forward;
}

/**
 * @brief Gets the camera's right vector (world-space)
 * @return Camera's right vector (world-space)
 */
glm::vec3 OrbitCamera::GetRightVector()
{
	return m_right;
}

/**
 * @brief Gets the camera's up vector (world-space)
 * @return Camera's up vector (world-space)
 */
glm::vec3 OrbitCamera::GetUpVector()
{
	return m_up;
}

/**
 * @brief Sets the world's up vector
 * @param[in] worldUp World's up vector
 */
void OrbitCamera::SetWorldUpVector(const glm::vec3& worldUp)
{
	m_worldUp = worldUp;
	UpdateVectors();
}

/**
 * @brief Gets the world up vector
 * @return World up vector
 */
glm::vec3 OrbitCamera::GetWorldUpVector()
{
	return m_worldUp;
}

/**
 * @brief Gets the view matrix for this camera
 * @return View matrix
 */
glm::mat4 OrbitCamera::GetViewMatrix()
{
	return glm::lookAt(m_position, m_pivotPosition, m_worldUp);
}

/**
 * @brief Gets the projection matrix for this camera
 * @return Projection matrix
 */
glm::mat4 OrbitCamera::GetProjectionMatrix()
{
	// TODO: Make the near and far values adjustable
	return glm::perspective(glm::radians(m_fov), m_aspectRatio, 0.1f, 100.0f);
}

/**
 * @brief Updates the forward, right, and up vectors
 */
void OrbitCamera::UpdateVectors()
{
    m_position.x = m_orbitDistance * glm::cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_position.y = m_orbitDistance * glm::sin(glm::radians(m_pitch));
    m_position.z = m_orbitDistance * glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_position += m_pivotPosition;

    m_forward = glm::normalize(m_pivotPosition - m_position);

	m_right = glm::cross(m_forward, m_worldUp);
	m_right = glm::normalize(m_right);

	m_up = glm::cross(m_right, m_forward);
}

