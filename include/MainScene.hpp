#pragma once

#include "Engine/Geometry/BoundingVolumes/AABB.hpp"
#include "Engine/Graphics/OrbitCamera.hpp"
#include "Engine/Graphics/Renderer.hpp"

#include "Engine/Font/Font.hpp"
#include "Engine/UI/Text.hpp"

#include "Engine/SceneBase.hpp"

#include "Chunk.hpp"
#include "Terrain.hpp"

#include <glm/glm.hpp>

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
        void ThreadJob(int threadIndex, std::function<float(float, float, float)> signedDistanceFunc);

        /**
         * @brief Update chunks
         */
        void UpdateChunks();

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
        std::queue<Chunk*> m_threadJobQueue;

        /**
         * Mutex for the job queue
         */
        std::mutex m_threadJobQueueMutex;

        /**
         * List of chunks
         */
        std::vector<Chunk*> m_loadedChunks;

        /**
         * Mutex for the chunk list
         */
        std::mutex m_chunkListMutex;

        /**
         * List of worker threads
         */
        std::vector<std::thread> m_workerThreads;

        /**
         * Chunk size
         */
        float m_chunkSize;

        /**
         * Voxel size
         */
        float m_voxelSize;

        /**
         * Number of chunks to render in all axes
         */
        glm::ivec3 m_chunkRenderDistance;

        /**
         * Is our application done?
         */
        bool m_isDone;

        /**
         * Is this our first chunk update?
         */
        bool m_firstChunkUpdate;

        /**
         * Previous chunk index that we were in last frame
         */
        glm::ivec3 m_prevChunkIndex;


        // --- UI ---
        
        /**
         * Font to be used to display text on the screen
         */
        Font* m_font;

        /**
         * Text UI that will display debug messages
         */
        Text* m_debugText;
    };
}

