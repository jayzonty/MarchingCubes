#include "Engine/Graphics/MeshBuilder.hpp"

/**
 * @brief Constructor
 */
MeshBuilder::MeshBuilder()
    : m_vertices()
	, m_indices()
{
}

/**
 * @brief Destructor
 */
MeshBuilder::~MeshBuilder()
{
}

/**
 * @brief Clear all stored data
 */
void MeshBuilder::Clear()
{
    m_vertices.clear();
	m_indices.clear();
}

/**
 * @brief Adds a vertex to the mesh builder
 * @param[in] vertex Vertex to add
 */
void MeshBuilder::AddVertex(const Vertex& vertex)
{
    m_vertices.push_back(vertex);
}

/**
 * @brief Adds a vertex index to the mesh builder
 * @param[in] vertexIndex Vertex index to add
 */
void MeshBuilder::AddVertexIndex(const GLuint& vertexIndex)
{
    m_indices.push_back(vertexIndex);
}

/**
 * @brief Sets list of vertex indices
 * @param[in] indices New list of vertex indices
 */
void MeshBuilder::SetIndices(const std::vector<GLuint>& indices)
{
	m_indices = indices;
}

/**
 * @brief Builds the mesh using prior provided information, and stores the result to the provided mesh reference.
 * @param[in] mesh Reference to the mesh where the result will be stored
 */
void MeshBuilder::BuildMesh(Mesh& mesh)
{
	if (mesh.m_vao == 0)
	{
		glGenVertexArrays(1, &mesh.m_vao);
	}
	glBindVertexArray(mesh.m_vao);

	if (mesh.m_vbo == 0)
	{
		glGenBuffers(1, &mesh.m_vbo);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_DYNAMIC_DRAW);

	if (mesh.m_ebo == 0)
	{
		glGenBuffers(1, &mesh.m_ebo);
	}

	mesh.m_numIndices = static_cast<int>(m_indices.size());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

	glBindVertexArray(0);
}

