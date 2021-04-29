#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstdio>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Font/Font.hpp>

#include <vector>

/**
 * Text display class
 */
class Text
{
public:
    /**
     * Horizontal alignment enum
     */
	enum class HorizontalAlignment
	{
		LEFT,
		CENTER,
		RIGHT
	};

private:
    /**
     * Struct containing info for a line of text
     */
	struct Line {
        /**
         * Text string
         */
		std::string str;

        /**
         * Line width
         */
		int width;
	};

    /**
     * VBO
     */
	GLuint m_vbo;

    /**
     * VAO
     */
	GLuint m_vao;

    /**
     * Reference to the font
     */
	Font* m_font;

    /**
     * Shader
     */
	ShaderProgram m_shader;

    /**
     * Text color
     */
	glm::vec4 m_color;

    /**
     * List of line info
     */
	std::vector<Line> m_lines;

    /**
     * Amount of line spacing
     */
	float m_lineSpacing;

    /**
     * Line height
     */
	int m_lineHeight;

    /**
     * X-position
     */
	float m_x;

    /**
     * Y-position
     */
	float m_y;

    /**
     * Horizontal alignment
     */
	HorizontalAlignment m_horizontalAlign;

    /**
     * Width
     */
	int m_width;

    /**
     * Height
     */
	int m_height;

public:
    /**
     * @brief Constructor
     */
	Text();

    /**
     * @brief Destructor
     */
	~Text();

    /**
     * @brief Sets the line spacing
     * @param[in] lineSpacing Line spacing
     */
	void SetLineSpacing(float lineSpacing);

    /**
     * @brief Sets the text font
     * @param[in] font Font that this text UI will use
     */
	void SetFont(Font& font);

    /**
     * @brief Sets the string of text to display
     * @param[in] str String to display
     */
	void SetString(const std::string& str);

    /**
     * @brief Sets the text color
     * @param[in] color Text color
     */
	void SetColor(const glm::vec4& color);

    /**
     * @brief Sets the position of the text.
     * @param[in] x X-position
     * @param[in] y Y-position
     */
	void SetPosition(const float& x, const float& y);

    /**
     * @brief Gets the text position
     * @return Text position
     */
	glm::vec2 GetPosition() const;

    /**
     * @brief Draws the text
     * @param[in] projectionMatrix Projection matrix
     */
	void Draw(const glm::mat4& projectionMatrix);

    /**
     * @brief Computes the size of the bounds of the text
     * @param[out] Variable to set the width to
     * @param[out] Variable to set the height to
     */
	void ComputeSize(int* width, int* height);

    /**
     * Sets the horizontal alignment of the text
     * @param[in] horizontalAlign Horizontal alignment
     */
	void SetHorizontalAlignment(HorizontalAlignment horizontalAlign);
};

