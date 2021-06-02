#pragma once

#include "Camera.hpp"

#include <glm/glm.hpp>

/**
 * First person camera
 */
class FirstPersonCamera : public Camera
{
public:
    /**
     * @brief Constructor
     */
    FirstPersonCamera();

    /**
     * @brief Destructor
     */
    ~FirstPersonCamera();

    /**
     * @brief Sets this camera's position
     * @param[in] position New position
     */
    void SetPosition(const glm::vec3& position);

    /**
     * @brief Sets the yaw angle of this camera
     * @param[in] yaw Yaw angle
     */
    void SetYaw(const float& yaw);

    /**
     * @brief Gets the yaw angle of this camera
     * @return Yaw angle
     */
    float GetYaw();

    /**
     * @brief Sets the pitch angle of this camera
     * @param[in] pitch Pitch angle
     */
    void SetPitch(const float& pitch);

    /**
     * @brief Gets the pitch angle of this camera
     * @return Pitch angle
     */
    float GetPitch();

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
     * Yaw
     */
    float m_yaw;

    /**
     * Pitch
     */
    float m_pitch;

};

