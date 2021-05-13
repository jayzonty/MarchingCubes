#include "SandboxScene.hpp"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

/**
 * @brief Constructor
 */
SandboxScene::SandboxScene()
    : m_font()
    , m_text()
{
}

/**
 * @brief Destructor
 */
SandboxScene::~SandboxScene()
{
}

/**
 * @brief Starts the scene
 */
void SandboxScene::Start()
{
    m_font.Load("resources/fonts/SourceCodePro/SourceCodePro-Regular.ttf");
    m_font.SetSize(18);
    if (!m_font.IsLoaded())
    {
        std::cout << "Failed to load font!" << std::endl;
    }

    m_text.SetFont(m_font);
    m_text.SetString("Sandbox text");
    m_text.SetPosition(0.0f, 0.0f);
    m_text.SetColor(glm::vec4(1.0f));

    int width, height;
    m_text.ComputeSize(&width, &height);
}

/**
 * @brief Finishes the scene
 */
void SandboxScene::Finish()
{
}

/**
 * @brief Updates the scene state
 * @param[in] deltaTime Time elapsed since the last frame
 */
void SandboxScene::Update(float deltaTime)
{
}

/**
 * @brief Draws the scene
 */
void SandboxScene::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    m_text.Draw(projectionMatrix);
}

