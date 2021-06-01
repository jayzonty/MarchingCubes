#pragma once

#include "Vertex.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <vector>

/**
 * Renderer
 */
class Renderer
{
private:
    /**
     * VBO
     */
    GLuint m_vbo;

    /**
     * VAO
     */
    GLuint m_vao;

    /**
     * EBO
     */
    GLuint m_ebo;

    /**
     * Maximum number of vertices
     */
    int m_maxNumVertices;

    /**
     * Maximum number of vertex indices
     */
    int m_maxNumVertexIndices;

public:
    /**
     * @brief Constructor
     */
    Renderer();

    /**
     * @brief Destructor
     */
    ~Renderer();

    /**
     * @brief Initialize the renderer
     * @param[in] maxVertices Maximum number of vertices
     * @param[in] maxVertexIndices Maximum number of vertex indices
     */
    void Initialize(const int& maxNumVertices, const int& maxNumVertexIndices);

    /**
     * @brief Cleanup resources
     */
    void Cleanup();

    /**
     * @brief Draw lines from the provided vertices
     * @param[in] vertices Vertex list
     */
    void DrawLines(const std::vector<Vertex>& vertices);

    /**
     * @brief Draw the provided vertices as triangles
     * @param[in] vertices Vertex list
     */
    void DrawTriangles(const std::vector<Vertex>& vertices);

    /**
     * @brief Draw the outline of a box (not solid box)
     * @param[in] center Box center
     * @param[in] halfExtents Box half-extents
     * @param[in] color Color
     */
    void DrawBoxOutline(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec4& color);
};

