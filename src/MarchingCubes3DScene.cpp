#include "MarchingCubes3DScene.hpp"

#include "Cube.hpp"
#include "Sphere.hpp"

#include "Engine/Input.hpp"
#include "Engine/ResourceManager.hpp"
#include "Engine/Graphics/ShaderProgram.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>

namespace MarchingCubes
{
    /**
     * @brief Constructor
     */
    MarchingCubes3DScene::MarchingCubes3DScene()
    	: SceneBase()
        , m_renderer()
        , m_sphereVertices()
        , m_cubeVertices()
    {
    }
    
    /**
     * @brief Destructor
     */
    MarchingCubes3DScene::~MarchingCubes3DScene()
    {
    }
    
    /**
     * @brief Starts the scene
     */
    void MarchingCubes3DScene::Start()
    {
        ResourceManager::CreateShader("resources/shaders/main.vsh", "resources/shaders/main.fsh", "main");

        m_renderer.Initialize(100000, 100000);

        glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
        float cellSize = 0.02f;

        Sphere sphere = {};
        sphere.radius = 0.5f;
        sphere.center[0] = -2.0f; sphere.center[1] = 0.0f; sphere.center[2] = 0.0f;

        m_sphereVertices.clear();

        AABB sphereBounds;
        sphere.GetBounds(sphereBounds);
        
        std::vector<Triangle> triangles;
        MarchingCubes(std::bind(&Sphere::GetSignedDistnaceToSurface, &sphere, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), sphereBounds, cellSize, triangles);
        for (size_t i = 0; i < triangles.size(); ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                m_sphereVertices.emplace_back();
                m_sphereVertices.back().position = triangles[i].vertices[j];
                m_sphereVertices.back().color = color;
                m_sphereVertices.back().normal = triangles[i].GetNormal();
            }
        }

        Cube cube = {};
        cube.radius = 0.5f;
        cube.center[0] = 0.0f; cube.center[1] = 0.0f; cube.center[2] = 0.0f;

        m_cubeVertices.clear();

        AABB cubeBounds;
        cube.GetBounds(cubeBounds);

        triangles.clear();
        MarchingCubes(std::bind(&Cube::GetSignedDistnaceToSurface, &cube, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), cubeBounds, cellSize, triangles);
        for (size_t i = 0; i < triangles.size(); ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                m_cubeVertices.emplace_back();
                m_cubeVertices.back().position = triangles[i].vertices[j];
                m_cubeVertices.back().color = color;
                m_cubeVertices.back().normal = triangles[i].GetNormal();
            }
        }

        std::cout << m_sphereVertices.size() + m_cubeVertices.size() << std::endl;
    }
    
    /**
     * @brief Finishes the scene
     */
    void MarchingCubes3DScene::Finish()
    {
        m_renderer.Cleanup();
    }
    
    /**
     * @brief Updates the scene state
     * @param[in] deltaTime Time elapsed since the last frame
     */
    void MarchingCubes3DScene::Update(float deltaTime)
    {
    }
    
    /**
     * @brief Draws the scene
     */
    void MarchingCubes3DScene::Draw()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
        glEnable(GL_DEPTH_TEST);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        ShaderProgram* mainShader = ResourceManager::GetShader("main");
        if (mainShader != nullptr)
        {
            mainShader->Use();

            glm::vec3 cameraPosition(0.0f);
            float orbitDistance = 3.0f;
            float yaw = static_cast<float>(glfwGetTime()) * 20.0f;
            float pitch = 0.0f;
            cameraPosition.x = orbitDistance * glm::cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraPosition.y = orbitDistance * glm::sin(glm::radians(pitch));
            cameraPosition.z = orbitDistance * glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

            glm::mat4 projMatrix = glm::perspective(glm::radians(90.0f), 800.0f/ 600.0f, 0.1f, 100.0f);
            glm::mat4 viewMatrix = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 modelMatrix = glm::mat4(1.0f);// glm::rotate(glm::mat4(1.0f), static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 mvpMatrix = projMatrix * viewMatrix * modelMatrix;
            mainShader->SetUniformMatrix4fv("mvpMatrix", false, glm::value_ptr(mvpMatrix));
            mainShader->SetUniformMatrix4fv("modelMatrix", false, glm::value_ptr(modelMatrix));

            mainShader->SetUniform3f("lightPos", 0.0f, 2.0f, 0.0f);

            m_renderer.DrawTriangles(m_sphereVertices);
            m_renderer.DrawTriangles(m_cubeVertices);
        }
    }

    /**
     * Perform the marching cube algorithm
     * @param[in] signedDistanceFunc Signed distance function
     * @param[in] bounds Shape bounds
     * @param[in] cellSize Cell size
     * @param[out] outputTriangles Vector where the triangles will be placed
     */
    void MarchingCubes3DScene::MarchingCubes(std::function<float(float, float, float)> signedDistanceFunc, const AABB& bounds, float cellSize, std::vector<Triangle>& outputTriangles)
    {
        for (float x = bounds.min[0]; x < bounds.max[0]; x += cellSize)
        {
            for (float y = bounds.min[1]; y < bounds.max[1]; y += cellSize)
            {
                for (float z = bounds.min[2]; z > bounds.max[2]; z -= cellSize) // Right-handed system
                {
                    GetCellTriangles(signedDistanceFunc, x, y, z, cellSize, outputTriangles);
                }
            }
        }
    }

    /**
     * Gets the cell triangles based on the resulting cell configuration calculated from the provided function
     * @param[in] signedDistanceFunc Signed distance function
     * @param[in] cellX X-position of the cell
     * @param[in] cellY Y-position of the cell
     * @param[in] cellZ Z-position of the cell
     * @param[in] cellSize Cell size
     * @param[out] outputTriangles Vector where the triangles will be placed 
     */
    void MarchingCubes3DScene::GetCellTriangles(std::function<float(float, float, float)> signedDistanceFunc, float cellX, float cellY, float cellZ, float cellSize, std::vector<Triangle>& outputTriangles)
    {
        float values[8];
        for (int i = 0; i < 8; ++i)
        {
            values[i] = signedDistanceFunc(cellX + vertexPositionOffsets[i].x * cellSize,
                                           cellY + vertexPositionOffsets[i].y * cellSize,
                                           cellZ + vertexPositionOffsets[i].z * cellSize);
        }

        int caseIndex = 0;
        for (int i = 0; i < 8; ++i)
        {
            if (values[i] > 0.0f)
            {
                caseIndex |= 1 << i;
            }
        }

        unsigned char caseClass = regularCellClass[caseIndex];

        RegularCellData cellData = regularCellData[caseClass];
        if (cellData.GetVertexCount() == 0)
        {
            return;
        }

        Triangle triangle;
        for (int i = 0; i < cellData.GetTriangleCount(); ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                int index = cellData.vertexIndex[i * 3 + j];

                int ev0 = (regularVertexData[caseIndex][index] & 0xF0) >> 4;
                int ev1 = regularVertexData[caseIndex][index] & 0x0F;

                glm::vec3 vPos = ((vertexPositionOffsets[ev0] + vertexPositionOffsets[ev1]) / 2.0f) * cellSize;
                vPos.x += cellX;
                vPos.y += cellY;
                vPos.z += cellZ;
                triangle.vertices[j] = vPos;
            }
            outputTriangles.push_back(triangle);
        }
    }
}
