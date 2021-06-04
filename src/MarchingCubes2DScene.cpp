#include "MarchingCubes2DScene.hpp"

#include "Engine/Input.hpp"
#include "Engine/ResourceManager.hpp"
#include "Engine/Graphics/ShaderProgram.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <cmath>

namespace MarchingCubes
{
    /**
     * @brief Constructor
     */
    MarchingCubes2DScene::MarchingCubes2DScene()
    	: SceneBase()
        , m_renderer()
        , m_circleVertices()
    {
    }
    
    /**
     * @brief Destructor
     */
    MarchingCubes2DScene::~MarchingCubes2DScene()
    {
    }
    
    /**
     * @brief Starts the scene
     */
    void MarchingCubes2DScene::Start()
    {
        ShaderCreateInfo shaderCreateInfo;
        shaderCreateInfo.vertexShaderFilePath = "resources/shaders/main.vsh";
        shaderCreateInfo.fragmentShaderFilePath = "resources/shaders/main.fsh";
        ResourceManager::CreateShader("main", shaderCreateInfo);

        m_renderer.Initialize(100000, 100000);

        m_circleVertices.clear();

        float minX = -400.0f, maxX = 400.0f, minY = -300.0f, maxY = 300.0f;
        float cellSize = 5.0f;
        std::vector<Edge> edges;

        glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
        for (float x = minX; x <= maxX; x += cellSize)
        {
            for (float y = minY; y <= maxY; y += cellSize)
            {
                edges.clear();
                GetCellEdges(std::bind(&MarchingCubes2DScene::CircleFunc, this, std::placeholders::_1, std::placeholders::_2), x, y, cellSize, edges);
                
                for (size_t i = 0; i < edges.size(); ++i)
                {
                    m_circleVertices.emplace_back();
                    m_circleVertices.back().position = edges[i].v0;
                    m_circleVertices.back().color = color;
                    
                    m_circleVertices.emplace_back();
                    m_circleVertices.back().position = edges[i].v1;
                    m_circleVertices.back().color = color;
                }
            }
        }
    }
    
    /**
     * @brief Finishes the scene
     */
    void MarchingCubes2DScene::Finish()
    {
        m_renderer.Cleanup();
    }
    
    /**
     * @brief Updates the scene state
     * @param[in] deltaTime Time elapsed since the last frame
     */
    void MarchingCubes2DScene::Update(float deltaTime)
    {
    }
    
    /**
     * @brief Draws the scene
     */
    void MarchingCubes2DScene::Draw()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        ShaderProgram* mainShader = ResourceManager::GetShader("main");
        if (mainShader != nullptr)
        {
            mainShader->Use();

            glm::mat4 projMatrix = glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f);
            glm::mat4 viewMatrix = glm::mat4(1.0f);
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            glm::mat4 mvpMatrix = projMatrix * viewMatrix * modelMatrix;
            mainShader->SetUniformMatrix4fv("mvpMatrix", false, glm::value_ptr(mvpMatrix));

            m_renderer.DrawLines(m_circleVertices);
        }
    }

    /**
     * Gets the cell edges based on the resulting cell configuration from the provided function
     * @param[in] func Function
     * @param[in] cellX X-position of the cell
     * @param[in] cellY Y-position of the cell
     * @param[in] cellSize Cell size
     * @param[out] outputEdges Vector where the edges will be placed 
     */
    void MarchingCubes2DScene::GetCellEdges(std::function<float(float, float)> func, float cellX, float cellY, float cellSize, std::vector<Edge>& outputEdges)
    {
        float lowerLeft = func(cellX, cellY);
        float lowerRight = func(cellX + cellSize, cellY);
        float upperRight = func(cellX + cellSize, cellY + cellSize);
        float upperLeft = func(cellX, cellY + cellSize);

        int configuration = 0;
        if (lowerLeft > 0.0f)
        {
            configuration |= 1;
        }
        if (lowerRight > 0.0f)
        {
            configuration |= 2;
        }
        if (upperRight > 0.0f)
        {
            configuration |= 4;
        }
        if (upperLeft > 0.0f)
        {
            configuration |= 8;
        }

        switch (configuration)
        {
            case 0:
                break;

            case 1:
                outputEdges.emplace_back(glm::vec3(cellX, cellY + cellSize / 2.0f, 0.0f), glm::vec3(cellX + cellSize / 2.0f, cellY, 0.0f));
                break;

            case 2:
                outputEdges.emplace_back(glm::vec3(cellX + cellSize / 2.0f, cellY, 0.0f), glm::vec3(cellX + cellSize, cellY + cellSize / 2.0f, 0.0f));
                break;

            case 3:
                outputEdges.emplace_back(glm::vec3(cellX, cellY + cellSize / 2.0f, 0.0f), glm::vec3(cellX + cellSize, cellY + cellSize / 2.0f, 0.0f));
                break;

            case 4:
                outputEdges.emplace_back(glm::vec3(cellX + cellSize, cellY + cellSize / 2.0f, 0.0f), glm::vec3(cellX + cellSize / 2.0f, cellY + cellSize, 0.0f));
                break;

            case 5:
                outputEdges.emplace_back(glm::vec3(cellX, cellY + cellSize / 2.0f, 0.0f), glm::vec3(cellX + cellSize / 2.0f, cellY, 0.0f));
                outputEdges.emplace_back(glm::vec3(cellX + cellSize, cellY + cellSize / 2.0f, 0.0f), glm::vec3(cellX + cellSize / 2.0f, cellY + cellSize, 0.0f));
                break;

            case 6:
                outputEdges.emplace_back(glm::vec3(cellX + cellSize / 2.0f, cellY, 0.0f), glm::vec3(cellX + cellSize / 2.0f, cellY + cellSize, 0.0f));
                break;

            case 7:
            case 8: // Ambiguous case with configuration 7
                outputEdges.emplace_back(glm::vec3(cellX + cellSize / 2.0f, cellY + cellSize, 0.0f), glm::vec3(cellX, cellY + cellSize / 2.0f, 0.0f));
                break;

            case 9: // Ambiguous case with configuration 6
                outputEdges.emplace_back(glm::vec3(cellX + cellSize / 2.0f, cellY, 0.0f), glm::vec3(cellX + cellSize / 2.0f, cellY + cellSize, 0.0f));
                break;

            case 10:
                outputEdges.emplace_back(glm::vec3(cellX + cellSize / 2.0f, cellY, 0.0f), glm::vec3(cellX + cellSize, cellY + cellSize / 2.0f, 0.0f));
                outputEdges.emplace_back(glm::vec3(cellX + cellSize / 2.0f, cellY + cellSize, 0.0f), glm::vec3(cellX, cellY + cellSize / 2.0f, 0.0f));
                break;

            case 11:
                outputEdges.emplace_back(glm::vec3(cellX + cellSize, cellY + cellSize / 2.0f, 0.0f), glm::vec3(cellX + cellSize / 2.0f, cellY + cellSize, 0.0f));
                break;

            case 12: // Ambiguous case with configuration 3
                outputEdges.emplace_back(glm::vec3(cellX, cellY + cellSize / 2.0f, 0.0f), glm::vec3(cellX + cellSize, cellY + cellSize / 2.0f, 0.0f));
                break;

            case 13: // Ambiguous case with configuration 2
                outputEdges.emplace_back(glm::vec3(cellX + cellSize / 2.0f, cellY, 0.0f), glm::vec3(cellX + cellSize, cellY + cellSize / 2.0f, 0.0f));
                break;

            case 14: // Ambiguous case with configuration 1
                outputEdges.emplace_back(glm::vec3(cellX, cellY + cellSize / 2.0f, 0.0f), glm::vec3(cellX + cellSize / 2.0f, cellY, 0.0f));
                break;

            case 15:
                break;
        }
    }

    /**
     * Function for a circle.
     * Right now, the radius of the circle is hard-coded into the function
     * @param[in] x X-value
     * @param[in] y Y-value
     * @return Result upon evaluating the function
     */
    float MarchingCubes2DScene::CircleFunc(float x, float y)
    {
        return 200.0f - sqrtf(x * x + y * y);
    }

    /**
     * Sin function.
     * Right now, the amplitude and the frequency are hard-coded into the function
     * @param[in] x X-value
     * @param[in] y Y-value
     * @return Result upon evaluating the function
     */
    float MarchingCubes2DScene::SinFunc(float x, float y)
    {
        return 100.0f * sinf(x / 20.0f) - y;
    }
}
