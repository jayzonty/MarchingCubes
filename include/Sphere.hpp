#pragma once

#include "Engine/Geometry/BoundingVolumes/AABB.hpp"

#include <cmath>

/**
 * Sphere struct containing data and function
 */
struct Sphere
{
    /**
     * Sphere radius
     */
    float radius;

    /**
     * Sphere center (x, y, z)
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
        return radius - sqrtf(powf(x - center[0], 2.0f) + 
                              powf(y - center[1], 2.0f) + 
                              powf(z - center[2], 2.0f));
    }

    /**
     * Gets the bounds of the sphere
     * @param[out] bounds Bounds of the sphere
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
