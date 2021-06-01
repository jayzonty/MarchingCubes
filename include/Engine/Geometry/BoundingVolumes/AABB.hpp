#pragma once

#include <glm/glm.hpp>

#include <cstdint>

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;

    bool Intersects(const AABB& other)
    {
        for (int32_t i = 0; i < 3; ++i)
        {
            if ((min[i] > other.max[i]) || (other.min[i] > max[i]))
            {
                return false;
            }
        }
        return true;
    }

    bool ContainsPoint(const glm::vec3& point)
    {
        for (int32_t i = 0; i < 3; ++i)
        {
            if ((point[i] < min[i]) || (point[i] > max[i]))
            {
                return false;
            }
        }
        return true;
    }
};
