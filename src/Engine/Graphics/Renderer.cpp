#include "Engine/Graphics/Renderer.hpp"

#include <cstddef>
#include <cstdint>

/**
 * @brief Constructor
 */
Renderer::Renderer()
    : m_vbo(0)
    , m_vao(0)
    , m_ebo(0)
{
}

/**
 * @brief Destructor
 */
Renderer::~Renderer()
{
}


/**
 * @brief Initialize the renderer
 * @param[in] maxVertices Maximum number of vertices
 * @param[in] maxVertexIndices Maximum number of vertex indices
 */
void Renderer::Initialize(const int& maxNumVertices, const int& maxNumVertexIndices)
{
    m_maxNumVertices = maxNumVertices;
    m_maxNumVertexIndices = maxNumVertexIndices;

    if (m_vao == 0)
    {
        glGenVertexArrays(1, &m_vao);
    }
    glBindVertexArray(m_vao);

    if (m_vbo == 0)
    {
        glGenBuffers(1, &m_vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * maxNumVertices, nullptr, GL_DYNAMIC_DRAW);

    if (m_ebo == 0)
    {
        glGenBuffers(1, &m_ebo);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * maxNumVertexIndices, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
}

/**
 * @brief Cleanup resources
 */
void Renderer::Cleanup()
{
    glDeleteBuffers(1, &m_ebo);
    m_ebo = 0;

    glDeleteBuffers(1, &m_vbo);
    m_vbo = 0;

    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;
}

/**
 * @brief Draw lines from the provided vertices
 * @param[in] vertices Vertex list
 */
void Renderer::DrawLines(const std::vector<Vertex>& vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), vertices.data());

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, vertices.size());
}

/**
 * @brief Draw the provided vertices as triangles
 * @param[in] vertices Vertex list
 */
void Renderer::DrawTriangles(const std::vector<Vertex>& vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), vertices.data());

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

/**
 * @brief Draw the provided vertices as a triangle fan
 * @param[in] vertices Vertex list
 */
void Renderer::DrawTriangleFan(const std::vector<Vertex>& vertices)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), vertices.data());

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
}

/**
 * @brief Draw the outline of a box (not solid box)
 * @param[in] center Box center
 * @param[in] halfExtents Box half-extents
 * @param[in] color Color
 */
void Renderer::DrawBoxOutline(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec4& color)
{
    std::vector<Vertex> vertices;

    float signX[] = { -1.0f,  1.0f,  1.0f, -1.0f };
    float signY[] = { -1.0f,  1.0f };
    float signZ[] = {  1.0f,  1.0f, -1.0f, -1.0f };

    glm::vec3 positions[8];
    for (int32_t i = 0; i < 8; ++i)
    {
        positions[i].x = center.x + halfExtents.x * signX[i % 4];
        positions[i].y = center.y + halfExtents.y * signY[i / 4];
        positions[i].z = center.z + halfExtents.z * signZ[i % 4];
    }

    for (int32_t i = 0; i < 8; ++i)
    {
        int32_t offset = (i / 4);
        offset *= 4;

        vertices.emplace_back();
        vertices.back().position = positions[i];
        vertices.back().color = color;

        vertices.emplace_back();
        vertices.back().position = positions[offset + ((i + 1) % 4)];
        vertices.back().color = color;
    }
    for (int32_t i = 0; i < 4; ++i)
    {
        vertices.emplace_back();
        vertices.back().position = positions[i];
        vertices.back().color = color;

        vertices.emplace_back();
        vertices.back().position = positions[i + 4];
        vertices.back().color = color;
    }

    DrawLines(vertices);
}

