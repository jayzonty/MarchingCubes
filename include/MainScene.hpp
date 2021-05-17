#pragma once

#include "Engine/Geometry/BoundingVolumes/AABB.hpp"
#include "Engine/Graphics/OrbitCamera.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Graphics/Vertex.hpp"

#include "Engine/SceneBase.hpp"

#include "Terrain.hpp"

#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace MarchingCubes
{
    /**
     * Main scene
     */
    class MainScene : public SceneBase
    {
    public:
        /**
         * @brief Constructor
         */
        MainScene();

        /**
         * @brief Destructor
         */
        ~MainScene();

        /**
         * @brief Starts the scene
         */
        void Start() override;

        /**
         * @brief Finishes the scene
         */
        void Finish() override;

        /**
         * @brief Updates the scene state
         * @param[in] deltaTime Time elapsed since last frame
         */
        void Update(float deltaTime) override;

        /**
         * @brief Draws the scene
         */
        void Draw() override;

    private:
        /**
         * @brief Routine to be done by each worker thread.
         * @param[in] threadIndex Thread index
         * @param[in] signedDistanceFunc Signed distance function
         * @param[in] voxelSize Voxel size
         */
        void ThreadJob(int threadIndex, std::function<float(float, float, float)> signedDistanceFunc, float voxelSize);

    private:
        /**
         * Terrain
         */
        Terrain m_terrain;

        /**
         * Renderer
         */
        Renderer m_renderer;

        /**
         * Orbit camera
         */
        OrbitCamera m_orbitCamera;

        /**
         * Queue containing the chunks that the threads still
         * need to work on
         */
        std::queue<AABB> m_threadJobQueue;

        /**
         * Mutex for the job queue
         */
        std::mutex m_threadJobQueueMutex;

        /**
         * List of vertices
         */
        std::vector<Vertex> m_vertices;

        /**
         * Mutex for the vertex list
         */
        std::mutex m_verticesMutex;

        /**
         * List of worker threads
         */
        std::vector<std::thread> m_workerThreads;
    };
}

