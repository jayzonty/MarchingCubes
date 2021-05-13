#pragma once

#include <glm/glm.hpp>

/**
 * Vertex data
 */
struct Vertex
{
    /**
     * Position
     */
    glm::vec3 position;

    /**
     * Color
     */
    glm::vec4 color;

    /**
     * Texture coordinates
     */
    glm::vec2 uv;

    /**
     * Normal
     */
    glm::vec3 normal;
};

