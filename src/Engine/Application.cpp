#include "Engine/Application.hpp"

#include "Engine/Input.hpp"
#include "Engine/ResourceManager.hpp"
#include "Engine/Time.hpp"

#include <iostream>

/**
 * @brief Constructor
 */
Application::Application()
	: m_window(nullptr)
	, m_currentScene(nullptr)
{
}

/**
 * @brief Destructor
 */
Application::~Application()
{
}

/**
 * @brief Run application
 */
void Application::Run()
{
	Initialize();

    ResourceManager::Initialize();

	// Take note of the current time before
	// running the render loop
	double prevTime = glfwGetTime();

	if (m_currentScene != nullptr)
	{
		m_currentScene->Start();
	}

	// Render loop
	while (!glfwWindowShouldClose(m_window))
	{
		// If escape is pressed, close the window
		if (Input::IsPressed(Input::Key::ESCAPE))
		{
			glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		}

		// For now, just use a basic delta time calculation
		double deltaTimeAsDouble = glfwGetTime() - prevTime;
		prevTime = glfwGetTime();
		float deltaTime = static_cast<float>(deltaTimeAsDouble);

        // Set the delta time field of the Time singleton class
        Time::GetInstance().m_deltaTime = deltaTime;

		if (m_currentScene != nullptr)
		{
			// Update current scene
			m_currentScene->Update(deltaTime);

			// Draw current scene
			m_currentScene->Draw();
		}

		// Tell GLFW to swap the screen buffer with the offscreen buffer
		glfwSwapBuffers(m_window);

		// Update input manager state before polling the new state
		Input::Prepare();

		// Poll for events
		glfwPollEvents();
	}

    if (m_currentScene != nullptr)
	{
		// Finish scene
		m_currentScene->Finish();
	}

    ResourceManager::Cleanup();

	Finish();
}

/**
 * @brief Sets the starting scene
 * @param[in] New starting scene
 */
void Application::SetStartingScene(SceneBase* scene)
{
	m_currentScene = scene;
}

/**
 * @brief Initialize application
 * @return True if initialization was successful. False otherwise
 */
bool Application::Initialize()
{
	// Initialize GLFW
	int glfwInitStatus = glfwInit();
	if (glfwInitStatus == GLFW_FALSE)
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return false;
	}

	// Tell GLFW that we prefer to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW that we prefer to use the modern OpenGL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Tell GLFW to create a window
	int windowWidth = 800;
	int windowHeight = 600;
	m_window = glfwCreateWindow(windowWidth, windowHeight, "Marching Cubes", nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return false;
	}

	// Tell GLFW to use the OpenGL context that was assigned to the window that we just created
	glfwMakeContextCurrent(m_window);

	// Set the callback function for when the framebuffer size changed
	glfwSetFramebufferSizeCallback(m_window, Application::FramebufferSizeChangedCallback);

	// Set the callback function for when a key was pressed
	glfwSetKeyCallback(m_window, Input::KeyCallback);

	// Register callback function for when a mouse button was pressed
	glfwSetMouseButtonCallback(m_window, Input::MouseButtonCallback);

    // Register callback function for when the mouse scroll wheel was scrolled
    glfwSetScrollCallback(m_window, Input::MouseScrollCallback);

	// Register callback function for when the mouse cursor's position changed
	glfwSetCursorPosCallback(m_window, Input::CursorCallback);

	// Register callback function for when the mouse cursor entered/left the window
	glfwSetCursorEnterCallback(m_window, Input::CursorEnterCallback);

	// TODO: Make this adjustable via the Input class
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Tell GLAD to load the OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return 1;
	}

	// Tell OpenGL the dimensions of the region where stuff will be drawn.
	// For now, tell OpenGL to use the whole screen
	glViewport(0, 0, windowWidth, windowHeight);

	return true;
}

/**
 * @brief Finish application
 */
void Application::Finish()
{
	// Re-enable the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Tell GLFW to clean itself up before exiting the application
	glfwTerminate();
}

/**
 * @brief Function for handling the event when the size of the framebuffer changed.
 * @param[in] window Reference to the window
 * @param[in] width New width
 * @param[in] height New height
 */
void Application::FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height)
{
	// Whenever the size of the framebuffer changed (due to window resizing, etc.),
	// update the dimensions of the region to the new size
	glViewport(0, 0, width, height);
}

