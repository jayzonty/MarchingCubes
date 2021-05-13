#pragma once

#include <glm/glm.hpp>

/**
 * Orbit camera
 */
class OrbitCamera
{
private:
    /**
     * Vertical field of view
     */
	float m_fov;

    /**
     * Aspect ratio
     */
	float m_aspectRatio;

    /**
     * Camera position
     */
	glm::vec3 m_position;

    /**
     * Pivot point that this camera is orbiting around
     */
    glm::vec3 m_pivotPosition;

    /**
     * Orbit distance
     */
    float m_orbitDistance;

    /**
     * Orbit yaw angle
     */
	float m_yaw;

    /**
     * Orbit pitch angle
     */
	float m_pitch;

    /**
     * Camera's forward direction, i.e., camera look direction
     */
	glm::vec3 m_forward;

    /**
     * Camera's "right" vector in world-space
     */
	glm::vec3 m_right;

    /**
     * Camera's "up" vector in world-space
     */
	glm::vec3 m_up;

    /**
     * Global "up" vector
     */
	glm::vec3 m_worldUp;

public:
    /**
     * @brief Constructor
     */
	OrbitCamera();

    /**
     * @brief Destructor
     */
	~OrbitCamera();

    /**
     * @brief Sets the camera's field of view
     * @param[in] fov New field of view (in degrees)
     */
	void SetFieldOfView(const float& fov);

    /**
     * @brief Gets the camera's field of view
     * @return Field of view (in degrees)
     */
	float GetFieldOfView();

    /**
     * @brief Sets the camera's aspect ratio
     * @param[in] aspectRatio Aspect ratio
     */
	void SetAspectRatio(const float& aspectRatio);

    /**
     * Gets the camera's aspect ratio
     * @return Aspect ratio
     */
	float GetAspectRatio();

    /**
     * @brief Sets the position that this camera is orbiting around
     * @param[in] pivotPosition Orbit pivot position
     */
	void SetPivotPosition(const glm::vec3& pivotPosition);

    /**
     * @brief Gets the position that this camera is orbiting around
     * @return Orbit pivot position
     */
    glm::vec3 GetPivotPosition();

    /**
     * @brief Gets the camera's position
     * @return Camera position
     */
	glm::vec3 GetPosition();

    /**
     * @brief Sets the camera's orbit distance
     * @param[in] orbit Distance Orbit distance
     */
    void SetOrbitDistance(const float& orbitDistance);

    /**
     * @brief Gets the camera's orbit distance
     * @return Camera's orbit distance
     */
    float GetOrbitDistance();

    /**
     * @brief Sets the camera's orbit yaw angle
     * @param[in] yaw Orbit yaw angle (in degrees)
     */
	void SetOrbitYaw(const float& yaw);

    /**
     * @brief Gets the camera's orbit yaw angle
     * @return Orbit yaw angle (in degrees)
     */
	float GetOrbitYaw();

    /**
     * @brief Sets the camera's orbit pitch angle
     * @param[in] pitch Orbit pitch angle (in degrees)
     */
	void SetOrbitPitch(const float& pitch);

    /**
     * @brief Gets the camera's pitch
     * @return Orbit pitch angle (in degrees)
     */
	float GetOrbitPitch();

    /**
     * @brief Gets the camera's forward vector (world-space)
     * @return Camera's forward vector (world-space)
     */
	glm::vec3 GetForwardVector();

    /**
     * @brief Gets the camera's right vector (world-space)
     * @return Camera's right vector (world-space)
     */
	glm::vec3 GetRightVector();

    /**
     * @brief Gets the camera's up vector (world-space)
     * @return Camera's up vector (world-space)
     */
	glm::vec3 GetUpVector();

    /**
     * @brief Sets the world's up vector
     * @param[in] worldUp World's up vector
     */
	void SetWorldUpVector(const glm::vec3& worldUp);

    /**
     * @brief Gets the world up vector
     * @return World up vector
     */
	glm::vec3 GetWorldUpVector();

    /**
     * @brief Gets the view matrix for this camera
     * @return View matrix
     */
	glm::mat4 GetViewMatrix();

    /**
     * @brief Gets the projection matrix for this camera
     * @return Projection matrix
     */
	glm::mat4 GetProjectionMatrix();

private:
    /**
     * @brief Updates the forward, right, and up vectors
     */
	void UpdateVectors();
};

