#pragma once

#include "Engine/Graphics/Renderer.hpp"
#include "Engine/Graphics/Vertex.hpp"

#include "Engine/SceneBase.hpp"

#include "Edge.hpp"

#include <functional>
#include <vector>

namespace MarchingCubes
{
    /**
     * Main scene
     */
    class MarchingCubes2DScene : public SceneBase
    {
    private:
        /**
         * Renderer
         */
        Renderer m_renderer;

        /**
         * Cached vertices of the circle
         */
        std::vector<Vertex> m_circleVertices;
    
    public:
        /**
         * @brief Constructor
         */
    	MarchingCubes2DScene();
    
        /**
         * @brief Destructor
         */
    	~MarchingCubes2DScene();
    
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
         * @param[in] deltaTime Time elapsed since the last frame
         */
    	void Update(float deltaTime) override;
    
        /**
         * @brief Draws the scene
         */
    	void Draw() override;

    private:
        /**
         * Gets the cell edges based on the resulting cell configuration from the provided function
         * @param[in] func Function
         * @param[in] cellX X-position of the cell
         * @param[in] cellY Y-position of the cell
         * @param[in] cellSize Cell size
         * @param[out] outputEdges Vector where the edges will be placed 
         */
        void GetCellEdges(std::function<float(float, float)> func, float cellX, float cellY, float cellSize, std::vector<Edge>& outputEdges);

        /**
         * Function for a circle.
         * Right now, the radius of the circle is hard-coded into the function
         * @param[in] x X-value
         * @param[in] y Y-value
         * @return Result upon evaluating the function
         */
        float CircleFunc(float x, float y);

        /**
         * Sin function.
         * Right now, the amplitude and the frequency are hard-coded into the function
         * @param[in] x X-value
         * @param[in] y Y-value
         * @return Result upon evaluating the function
         */
        float SinFunc(float x, float y);
    };
}

