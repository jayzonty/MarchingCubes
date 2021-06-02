#pragma once

#include "Camera.hpp"

#include <glm/glm.hpp>

/**
 * Orbit camera
 */
class OrbitCamera : public Camera
{
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
     * @brief Sets the point that this camera is orbiting around
     * @param[in] pivotPosition Orbit pivot point
     */
    void SetPivotPoint(const glm::vec3& pivotPoint);

    /**
     * @brief Gets the point that this camera is orbiting around
     * @return Orbit pivot point
     */
    glm::vec3 GetPivotPoint();

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
     * @brief Gets the view matrix for this camera
     * @return View matrix
     */
    virtual glm::mat4 GetViewMatrix() override;

protected:
    /**
     * @brief Update vectors
     */
    virtual void UpdateVectors() override;

private:
    /**
     * Pivot point that this camera is orbiting around
     */
    glm::vec3 m_pivotPoint;

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
};

