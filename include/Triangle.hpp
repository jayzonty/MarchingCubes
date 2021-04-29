#pragma once

#include <glm/glm.hpp>

/**
 * Triangle struct
 */
struct Triangle
{
    glm::vec3 vertices[3];

    glm::vec3 GetNormal() const
    {
        return glm::normalize(glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));
    }
};
