#pragma once

#include "Engine/Geometry/BoundingVolumes/AABB.hpp"
#include "Engine/Graphics/Vertex.hpp"

#include <glm/glm.hpp>

#include <vector>

struct Chunk
{
    glm::ivec3 indices;

    AABB bounds;

    std::vector<Vertex> meshVertices;

    bool isDone;
};
