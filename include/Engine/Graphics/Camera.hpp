#pragma once

#include <glm/glm.hpp>

/**
 * Camera
 */
class Camera
{
public:
    /**
     * @brief Constructor
     */
    Camera();

    /**
     * @brief Destructor
     */
    virtual ~Camera();

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
     * @brief Gets the camera's position
     * @return Camera position
     */
    glm::vec3 GetPosition();

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
     * @brief Sets the global up vector
     * @param[in] globalUp Global up vector
     */
    void SetGlobalUpVector(const glm::vec3& globalUp);

    /**
     * @brief Gets the global up vector
     * @return Global up vector
     */
    glm::vec3 GetGlobalUpVector();

    /**
     * @brief Gets the view matrix for this camera
     * @return View matrix
     */
    virtual glm::mat4 GetViewMatrix() = 0;

    /**
     * @brief Gets the projection matrix for this camera
     * @return Projection matrix
     */
    glm::mat4 GetProjectionMatrix();

protected:
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
    glm::vec3 m_globalUp;

    /**
     * @brief Update vectors
     */
    virtual void UpdateVectors() = 0;
};

