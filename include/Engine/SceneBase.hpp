#pragma once

/**
 * Base class for all scenes
 */
class SceneBase
{
private:

public:
	/**
     * @brief Constructor
     */
	SceneBase()
	{
	}

	/**
     * @brief Destructor
     */
	virtual ~SceneBase()
	{
	}

	/**
     * @brief Starts the scene
     */
	virtual void Start() = 0;

	/**
     * @brief Finishes the scene
     */
	virtual void Finish() = 0;

	/**
     * @brief Updates the scene state
     * @param[in] deltaTime Time elapsed since the last frame
     */
	virtual void Update(float deltaTime) = 0;

	/**
     * @brief Draws the scene
     */
	virtual void Draw() = 0;
};
