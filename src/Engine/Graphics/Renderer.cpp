#include "Engine/Graphics/Renderer.hpp"

#include <cstddef>

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
