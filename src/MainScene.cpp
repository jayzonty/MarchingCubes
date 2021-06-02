#include "MainScene.hpp"

#include "MarchingCubes.hpp"
#include "Triangle.hpp"

#include "Engine/Input.hpp"
#include "Engine/Graphics/ShaderProgram.hpp"
#include "Engine/ResourceManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace MarchingCubes
{
    /**
     * @brief Constructor
     */
    MainScene::MainScene()
        : SceneBase()
        , m_terrain()
        , m_renderer()
        , m_camera()
        , m_threadJobQueue()
        , m_threadJobQueueMutex()
        , m_loadedChunks()
        , m_chunkListMutex()
        , m_workerThreads()
        , m_chunkSize(8.0f)
        , m_voxelSize(1.0f)
        , m_chunkRenderDistance(8, 8, 8)
        , m_isDone(false)
        , m_firstChunkUpdate(true)
        , m_prevChunkIndex(0)
        , m_font(nullptr)
        , m_debugText(nullptr)
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
        m_camera.SetAspectRatio(800.0f / 600.0f);
        m_camera.SetFieldOfView(90.0f);

        ResourceManager::CreateShader("resources/shaders/main.vsh", "resources/shaders/main.fsh", "main");
        ResourceManager::CreateShader("resources/shaders/color.vsh", "resources/shaders/color.fsh", "color");

        m_renderer.Initialize(1000000, 100000);

        glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

        m_loadedChunks.clear();

        std::function<float(float, float, float)> func = std::bind(&Terrain::DensityFunction, &m_terrain, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

        const int numWorkerThreads = 4;
        for (int i = 0; i < numWorkerThreads; ++i)
        {
            m_workerThreads.emplace_back(&MainScene::ThreadJob, this, i, func);
        }

        m_font = new Font();
        m_font->Load("resources/fonts/SourceCodePro/SourceCodePro-Regular.ttf");
        m_font->SetSize(18);

        m_debugText = new Text();
        m_debugText->SetFont(*m_font);
        m_debugText->SetColor(glm::vec4(1.0f));
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

        m_isDone = true;

        for (size_t i = 0; i < m_workerThreads.size(); ++i)
        {
            std::cout << "Waiting for thread " << i << " to finish." << std::endl;
            m_workerThreads[i].join();
        }
        m_workerThreads.clear();

        m_renderer.Cleanup();

        delete m_debugText;
        m_debugText = nullptr;

        delete m_font;
        m_font = nullptr;
    }

    /**
     * @brief Updates the scene state
     * @param[in] deltaTime Time elapsed since the last frame
     */
    void MainScene::Update(float deltaTime)
    {
        float movementSpeed = 5.0f;
        float movementDistance = movementSpeed * deltaTime;

        glm::vec3 forward = m_camera.GetForwardVector();

        glm::vec3 right = m_camera.GetRightVector();

        if (Input::IsDown(Input::Key::W))
        {
            m_camera.SetPosition(m_camera.GetPosition() + forward * movementDistance);
        }
        else if (Input::IsDown(Input::Key::S))
        {
            m_camera.SetPosition(m_camera.GetPosition() - forward * movementDistance);
        }
        if (Input::IsDown(Input::Key::A))
        {
            m_camera.SetPosition(m_camera.GetPosition() - right * movementDistance);
        }
        else if (Input::IsDown(Input::Key::D))
        {
            m_camera.SetPosition(m_camera.GetPosition() + right * movementDistance);
        }

        int mouseDeltaX, mouseDeltaY;
        Input::GetMouseDelta(&mouseDeltaX, &mouseDeltaY);

        float sensitivity = 1.0f;
        m_camera.SetYaw(m_camera.GetYaw() + mouseDeltaX * sensitivity);
        m_camera.SetPitch(glm::clamp(m_camera.GetPitch() - mouseDeltaY * sensitivity, -89.0f, 89.0f));

        /*if (Input::GetMouseScrollY() != 0)
        {
            float orbitDistance = m_camera.GetOrbitDistance();
            orbitDistance -= Input::GetMouseScrollY() * 0.1f;
            orbitDistance = std::max(orbitDistance, 0.1f);

            m_camera.SetOrbitDistance(orbitDistance);
        }*/

        UpdateChunks();
    }

    /**
     * @brief Draws the scene
     */
    void MainScene::Draw()
    {
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    
        glEnable(GL_DEPTH_TEST);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        ShaderProgram* mainShader = ResourceManager::GetShader("main");

        glm::mat4 projMatrix = m_camera.GetProjectionMatrix();
        glm::mat4 viewMatrix = m_camera.GetViewMatrix();
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        glm::mat4 mvpMatrix = projMatrix * viewMatrix * modelMatrix;

        mainShader->Use();

        mainShader->SetUniformMatrix4fv("mvpMatrix", false, glm::value_ptr(mvpMatrix));
        mainShader->SetUniformMatrix4fv("modelMatrix", false, glm::value_ptr(modelMatrix));

        glm::vec3 lightDir(0.0f, -1.0f, 1.0f);
        lightDir = glm::normalize(lightDir);
        mainShader->SetUniform3f("lightDir", lightDir.x, lightDir.y, lightDir.z);

        m_chunkListMutex.lock();
        for (size_t i = 0; i < m_loadedChunks.size(); ++i)
        {
            if (m_loadedChunks[i]->isDone)
            {
                m_renderer.DrawTriangles(m_loadedChunks[i]->meshVertices);
            }
        }
        m_chunkListMutex.unlock();

        /*ShaderProgram* colorShader = ResourceManager::GetShader("color");
        colorShader->Use();
        colorShader->SetUniformMatrix4fv("mvpMatrix", false, glm::value_ptr(mvpMatrix));
        m_chunkListMutex.lock();
        for (size_t i = 0; i < m_loadedChunks.size(); ++i)
        {
            if (m_loadedChunks[i]->isDone)
            {
                std::vector<Vertex> lines;
                for (size_t j = 0; j < m_loadedChunks[i]->meshVertices.size(); ++j)
                {
                    lines.emplace_back();
                    lines.back().position = m_loadedChunks[i]->meshVertices[j].position;
                    lines.back().color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

                    lines.emplace_back();
                    lines.back().position = m_loadedChunks[i]->meshVertices[j].position + m_loadedChunks[i]->meshVertices[j].normal;
                    lines.back().color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                }
                m_renderer.DrawLines(lines);
            }
        }
        m_chunkListMutex.unlock();*/

        int debugTextWidth, debugTextHeight;
        m_debugText->ComputeSize(&debugTextWidth, &debugTextHeight);
        m_debugText->SetPosition(10.0f, 10.0f);

        glm::mat4 orthoMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f);
        m_debugText->Draw(orthoMatrix);
    }

    /**
     * @brief Routine to be done by each worker thread.
     * @param[in] threadIndex Thread index
     * @param[in] signedDistanceFunc Signed distance function
     * @param[in] voxelSize Voxel size
     */
    void MainScene::ThreadJob(int threadIndex, std::function<float(float, float, float)> signedDistanceFunc)
    {
        std::cout << "Thread " << threadIndex << " created." << std::endl;

        while (!m_isDone)
        {
            Chunk* chunk = nullptr;

            m_threadJobQueueMutex.lock();
            bool isEmpty = m_threadJobQueue.empty();
            if (!isEmpty)
            {
                chunk = m_threadJobQueue.front();
                m_threadJobQueue.pop();
            }
            m_threadJobQueueMutex.unlock();

            if (!isEmpty)
            {
                std::vector<Triangle> triangles;
                MarchingCubes::GetInstance().GetMesh(signedDistanceFunc, chunk->bounds, m_voxelSize, triangles);

                for (size_t i = 0; i < triangles.size(); ++i)
                {
                    for (size_t j = 0; j < 3; ++j)
                    {
                        chunk->meshVertices.emplace_back();
                        chunk->meshVertices.back().position = triangles[i].vertices[j];
                        chunk->meshVertices.back().color = glm::vec4(1.0f);
                        chunk->meshVertices.back().normal = triangles[i].GetNormal();
                    }
                }
                chunk->isDone = true;
            }
        }

        std::cout << "Thread " << threadIndex << " done!" << std::endl;
    }

    /**
     * @brief Update chunks
     */
    void MainScene::UpdateChunks()
    {
        // Update chunks
        glm::vec3 pos = m_camera.GetPosition();
        glm::ivec3 currentChunkIndex = glm::floor(pos / m_chunkSize);

        // Update text
        std::stringstream debugTextStream;
        debugTextStream << "Position: " << std::setprecision(3) << pos.x << "," << pos.y << "," << pos.z << std::endl;
        debugTextStream << "Current chunk: " << currentChunkIndex.x << " " << currentChunkIndex.y << " " << currentChunkIndex.z << std::endl;
        int32_t numPendingChunks = 0, numCompletedChunks = 0;
        m_chunkListMutex.lock();
        for (size_t i = 0; i < m_loadedChunks.size(); ++i)
        {
            if (!m_loadedChunks[i]->isDone)
            {
                ++numPendingChunks;
            }
            else
            {
                ++numCompletedChunks;
            }
        }
        m_chunkListMutex.unlock();
        debugTextStream << "Pending chunks: " << numPendingChunks << std::endl;
        debugTextStream << "Completed chunks: " << numCompletedChunks << std::endl;
        m_debugText->SetString(debugTextStream.str());

        // TODO: https://stackoverflow.com/questions/66135217/how-to-subdivide-set-of-overlapping-aabb-into-non-overlapping-set-of-aabbs

        if (m_firstChunkUpdate || (currentChunkIndex != m_prevChunkIndex))
        {
            glm::ivec3 min = currentChunkIndex - m_chunkRenderDistance;
            glm::ivec3 max = currentChunkIndex + m_chunkRenderDistance;

            // We shrink the previous bounds just to prevent the new chunks that are
            // beside the previous bounds (but not intersecting) to not be excluded
            //
            // |---|-| <- New chunk should be loaded, but if prevBounds and chunkBounds
            // |   |-|    are beside each other, they would be "intersecting", which
            // |---|      means the new chunk will not be loaded.
            glm::ivec3 prevMin = m_prevChunkIndex - (m_chunkRenderDistance - 1);
            glm::ivec3 prevMax = m_prevChunkIndex + (m_chunkRenderDistance - 1);

            AABB targetBounds;
            targetBounds.min = glm::vec3(min) * m_chunkSize;
            targetBounds.max = glm::vec3(max) * m_chunkSize;

            m_chunkListMutex.lock();
            for (int32_t i = m_loadedChunks.size() - 1; i >= 0; --i)
            {
                if (m_loadedChunks[i]->isDone && !m_loadedChunks[i]->bounds.Intersects(targetBounds))
                {
                    delete m_loadedChunks[i];
                    m_loadedChunks[i] = m_loadedChunks.back();
                    m_loadedChunks.pop_back();
                }
            }
            m_chunkListMutex.unlock();

            AABB prevBounds;
            prevBounds.min = glm::vec3(prevMin) * m_chunkSize;
            prevBounds.max = glm::vec3(prevMax) * m_chunkSize;

            std::vector<Chunk*> chunksToGenerate;
            for (int32_t x = min.x; x < max.x; ++x)
            {
                for (int32_t y = min.y; y < max.y; ++y)
                {
                    for (int32_t z = min.z; z < max.z; ++z)
                    {
                        AABB chunkBounds;
                        chunkBounds.min = glm::vec3(x, y, z) * m_chunkSize;
                        chunkBounds.max = chunkBounds.min + m_chunkSize;
                        if (m_firstChunkUpdate || !prevBounds.Intersects(chunkBounds))
                        {
                            Chunk* chunk = new Chunk();

                            chunk->indices = glm::ivec3(x, y, z);
                            chunk->bounds = chunkBounds;
                            chunk->isDone = false;

                            m_chunkListMutex.lock();
                            m_loadedChunks.push_back(chunk);
                            m_chunkListMutex.unlock();

                            chunksToGenerate.push_back(chunk);
                        }
                    }
                }
            }

            m_threadJobQueueMutex.lock();
            for (size_t i = 0; i < chunksToGenerate.size(); ++i)
            {
                m_threadJobQueue.push(chunksToGenerate[i]);
            }
            m_threadJobQueueMutex.unlock();
        }

        m_firstChunkUpdate = false;
        m_prevChunkIndex = currentChunkIndex;
    }
}

