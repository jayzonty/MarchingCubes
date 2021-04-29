#pragma once

#include <glm/glm.hpp>

struct Edge
{
    glm::vec3 v0;
    glm::vec3 v1;

    Edge(const glm::vec3& a, const glm::vec3& b)
        : v0(a)
        , v1(b)
    {
    }
};
