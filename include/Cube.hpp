#pragma once

#include "Engine/Geometry/BoundingVolumes/AABB.hpp"

#include <algorithm>
#include <cmath>

struct Cube
{
    /**
     * Radius
     */
    float radius;

    /**
     * Cube center (x, y, z)
     */
    float center[3];

    /**
     * Returns the signed distance from the provided point to the surface.
     * @param[in] x X-value
     * @param[in] y Y-value
     * @param[in] z Z-value
     * @return Signed distance from the provided point to the surface.
     * The returned value is positive if the point is inside the sphere.
     * Otherwise, returns a negative distance.
     */
    float GetSignedDistnaceToSurface(float x, float y, float z)
    {
        return radius - std::max(std::max(std::abs(x), std::abs(y)), std::abs(z));
    }

    /**
     * Gets the bounds of the cube
     * @param[out] bounds Bounds of the cube
     */
    void GetBounds(AABB& bounds)
    {
        bounds.min[0] = bounds.min[1] = -radius;
        bounds.max[0] = bounds.max[1] = radius;

        bounds.min[2] = radius;
        bounds.max[2] = -radius;

        for (size_t i = 0; i < 3; ++i)
        {
            bounds.min[i] += center[i];
            bounds.max[i] += center[i];
        }
    }
};
