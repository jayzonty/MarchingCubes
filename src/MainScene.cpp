#include "MainScene.hpp"

#include "MarchingCubes.hpp"
#include "Triangle.hpp"

#include "Engine/Input.hpp"
#include "Engine/Graphics/ShaderProgram.hpp"
#include "Engine/ResourceManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace MarchingCubes
{
    /**
     * @brief Constructor
     */
    MainScene::MainScene()
        : SceneBase()
        , m_terrain()
        , m_renderer()
        , m_vertices()
        , m_orbitCamera()
        , m_threadJobQueue()
        , m_threadJobQueueMutex()
        , m_verticesMutex()
        , m_workerThreads()
    {
    }

    /**
     * @brief Destructor
     */
    MainScene::~MainScene()
    {
    }

    /**
     * @brief Starts the scene
     */
    void MainScene::Start()
    {
        m_orbitCamera.SetOrbitDistance(5.0f);
        m_orbitCamera.SetAspectRatio(800.0f / 600.0f);
        m_orbitCamera.SetFieldOfView(90.0f);

        ResourceManager::CreateShader("resources/shaders/main.vsh", "resources/shaders/main.fsh", "main");

        m_renderer.Initialize(1000000, 100000);

        glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

        m_vertices.clear();

        glm::vec3 min(-30.0f, -10.0f,  30.0f);
        glm::vec3 max( 30.0f,  10.0f, -30.0f);

        float chunkSize = 5.0f;
        for (float x = min.x; x <= max.x; x += chunkSize)
        {
            for (float z = min.z; z >= max.z; z -= chunkSize)
            {
                AABB aabb;
                aabb.min[0] = x; aabb.min[1] = min.y; aabb.min[2] = z;
                aabb.max[0] = x + chunkSize; aabb.max[1] = max.y; aabb.max[2] = z - chunkSize;

                m_threadJobQueue.push(aabb);
            }
        }

        float voxelSize = 0.25f;

        std::function<float(float, float, float)> func = std::bind(&Terrain::DensityFunction, &m_terrain, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        const int numWorkerThreads = 4;
        for (int i = 0; i < numWorkerThreads; ++i)
        {
            m_workerThreads.emplace_back(&MainScene::ThreadJob, this, i, func, voxelSize);
        }
    }

    /**
     * @brief Finishes the scene
     */
    void MainScene::Finish()
    {
        m_threadJobQueueMutex.lock();
        while (!m_threadJobQueue.empty())
        {
            m_threadJobQueue.pop();
        }
        m_threadJobQueueMutex.unlock();

        for (size_t i = 0; i < m_workerThreads.size(); ++i)
        {
            std::cout << "Waiting for thread " << i << " to finish." << std::endl;
            m_workerThreads[i].join();
        }
        m_workerThreads.clear();

        m_renderer.Cleanup();
    }

    /**
     * @brief Updates the scene state
     * @param[in] deltaTime Time elapsed since the last frame
     */
    void MainScene::Update(float deltaTime)
    {
        float movementSpeed = 5.0f;
        float movementDistance = movementSpeed * deltaTime;

        glm::vec3 forward = m_orbitCamera.GetForwardVector();
        forward.y = 0.0f;

        glm::vec3 right = m_orbitCamera.GetRightVector();
        right.y = 0.0f;

        if (Input::IsDown(Input::Key::W))
        {
            m_orbitCamera.SetPivotPosition(m_orbitCamera.GetPivotPosition() + forward * movementDistance);
        }
        else if (Input::IsDown(Input::Key::S))
        {
            m_orbitCamera.SetPivotPosition(m_orbitCamera.GetPivotPosition() - forward * movementDistance);
        }
        if (Input::IsDown(Input::Key::A))
        {
            m_orbitCamera.SetPivotPosition(m_orbitCamera.GetPivotPosition() - right * movementDistance);
        }
        else if (Input::IsDown(Input::Key::D))
        {
            m_orbitCamera.SetPivotPosition(m_orbitCamera.GetPivotPosition() + right * movementDistance);
        }

        if (Input::IsDown(Input::Button::MIDDLE_MOUSE))
        {
            float sensitivity = 2.0f;

            float yaw = m_orbitCamera.GetOrbitYaw() + Input::GetMouseDeltaX() * sensitivity;

            float pitch = std::max(m_orbitCamera.GetOrbitPitch() + Input::GetMouseDeltaY() * sensitivity, -89.0f);
            pitch = std::min(pitch, 89.0f);

            m_orbitCamera.SetOrbitYaw(yaw);
            m_orbitCamera.SetOrbitPitch(pitch);
        }

        if (Input::GetMouseScrollY() != 0)
        {
            float orbitDistance = m_orbitCamera.GetOrbitDistance();
            orbitDistance -= Input::GetMouseScrollY() * 0.1f;
            orbitDistance = std::max(orbitDistance, 0.1f);

            m_orbitCamera.SetOrbitDistance(orbitDistance);
        }
    }

    /**
     * @brief Draws the scene
     */
    void MainScene::Draw()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
        glEnable(GL_DEPTH_TEST);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        ShaderProgram* mainShader = ResourceManager::GetShader("main");
        if (mainShader != nullptr)
        {
            mainShader->Use();

            glm::vec3 cameraPosition(0.0f);
            float orbitDistance = 1.0f;
            float yaw = static_cast<float>(glfwGetTime()) * 20.0f;
            float pitch = 0.0f;
            cameraPosition.x = orbitDistance * glm::cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraPosition.y = orbitDistance * glm::sin(glm::radians(pitch));
            cameraPosition.z = orbitDistance * glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

            glm::mat4 projMatrix = m_orbitCamera.GetProjectionMatrix();
            glm::mat4 viewMatrix = m_orbitCamera.GetViewMatrix();
            glm::mat4 modelMatrix = glm::mat4(1.0f);// glm::rotate(glm::mat4(1.0f), static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 mvpMatrix = projMatrix * viewMatrix * modelMatrix;
            mainShader->SetUniformMatrix4fv("mvpMatrix", false, glm::value_ptr(mvpMatrix));
            mainShader->SetUniformMatrix4fv("modelMatrix", false, glm::value_ptr(modelMatrix));

            mainShader->SetUniform3f("lightPos", 0.0f, 2.0f, 0.0f);

            m_renderer.DrawTriangles(m_vertices);
        }
    }

    /**
     * @brief Routine to be done by each worker thread.
     * @param[in] threadIndex Thread index
     * @param[in] signedDistanceFunc Signed distance function
     * @param[in] voxelSize Voxel size
     */
    void MainScene::ThreadJob(int threadIndex, std::function<float(float, float, float)> signedDistanceFunc, float voxelSize)
    {
        std::cout << "Thread " << threadIndex << " created." << std::endl;

        bool isDone = false;
        while (!isDone)
        {
            AABB bounds;

            m_threadJobQueueMutex.lock();
            isDone = m_threadJobQueue.empty();
            if (!isDone)
            {
                bounds = m_threadJobQueue.front();
                m_threadJobQueue.pop();
            }
            m_threadJobQueueMutex.unlock();

            if (!isDone)
            {
                std::vector<Triangle> triangles;
                MarchingCubes::GetInstance().GetMesh(signedDistanceFunc, bounds, voxelSize, triangles);

                m_verticesMutex.lock();
                for (size_t i = 0; i < triangles.size(); ++i)
                {
                    for (size_t j = 0; j < 3; ++j)
                    {
                        m_vertices.emplace_back();
                        m_vertices.back().position = triangles[i].vertices[j];
                        m_vertices.back().color = glm::vec4(1.0f);
                        m_vertices.back().normal = triangles[i].GetNormal();
                    }
                }
                m_verticesMutex.unlock();
            }
        }

        std::cout << "Thread " << threadIndex << " done!" << std::endl;
    }
}

