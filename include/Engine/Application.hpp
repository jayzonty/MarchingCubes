#pragma once

#include "Engine/SceneBase.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * Application
 */
class Application
{
private:
	/**
     * Reference to the window object
     */
	GLFWwindow* m_window;
	
    /**
     * Current scene
     */
	SceneBase* m_currentScene;

public:
    /**
     * @brief Constructor
     */
	Application();

    /**
     * @brief Destructor
     */
	~Application();

    /**
     * @brief Run application
     */
	void Run();

     /**
      * @brief Sets the starting scene
      * @param[in] New starting scene
      */
     void SetStartingScene(SceneBase* scene);

private:
    /**
     * @brief Initialize application
     * @return True if initialization was successful. False otherwise
     */
	bool Initialize();

    /**
     * @brief Finish application
     */
	void Finish();

    /**
     * @brief Function for handling the event when the size of the framebuffer changed.
     * @param[in] window Reference to the window
     * @param[in] width New width
     * @param[in] height New height
     */
	static void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height);
};

