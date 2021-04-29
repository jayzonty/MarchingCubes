#pragma once

#include "Mesh.hpp"
#include "Vertex.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

/**
 * Class for handling building of a mesh and uploading to the GPU
 */
class MeshBuilder
{
private:
    /**
     * List of vertices
     */
    std::vector<Vertex> m_vertices;

    /**
     * List of indices
     */
	std::vector<GLuint> m_indices;

public:
    /**
     * @brief Constructor
     */
	MeshBuilder();

    /**
     * @brief Destructor
     */
	~MeshBuilder();

    /**
     * @brief Clear all stored data
     */
	void Clear();

    /**
     * @brief Adds a vertex to the mesh builder
     * @param[in] vertex Vertex to add
     */
	void AddVertex(const Vertex& vertex);

    /**
     * @brief Adds a vertex index to the mesh builder
     * @param[in] vertexIndex Vertex index to add
     */
    void AddVertexIndex(const GLuint& vertexIndex);

    /**
     * @brief Sets list of vertex indices
     * @param[in] indices New list of vertex indices
     */
	void SetIndices(const std::vector<GLuint>& indices);

    /**
     * @brief Builds the mesh using prior provided information, and stores the result to the provided mesh reference.
     * @param[in] mesh Reference to the mesh where the result will be stored
     */
	void BuildMesh(Mesh& mesh);
};
