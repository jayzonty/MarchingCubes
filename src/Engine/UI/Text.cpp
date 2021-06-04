#include "Engine/UI/Text.hpp"

#include "Engine/ResourceManager.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <string>

/**
 * @brief Constructor
 */
Text::Text()
	: m_vbo(0)
	, m_vao(0)
	, m_font(nullptr)
	, m_color(0.0f)
	, m_lineSpacing(0.0f)
	, m_lineHeight(0)
	, m_x(0)
	, m_y(0)
	, m_horizontalAlign(HorizontalAlignment::LEFT)
	, m_width(0)
	, m_height(0)
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);

	glBindVertexArray(0);

        ShaderCreateInfo shaderCreateInfo = {};
        shaderCreateInfo.vertexShaderFilePath = "resources/shaders/font.vsh";
        shaderCreateInfo.fragmentShaderFilePath = "resources/shaders/font.fsh";
	m_shader = ResourceManager::CreateShader("font", shaderCreateInfo);
}

/**
 * @brief Destructor
 */
Text::~Text()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

/**
 * @brief Sets the line spacing
 * @param[in] lineSpacing Line spacing
 */
void Text::SetLineSpacing(float lineSpacing)
{
	m_lineSpacing = lineSpacing;
}

/**
 * @brief Sets the text font
 * @param[in] font Font that this text UI will use
 */
void Text::SetFont(Font& font)
{
	m_font = &font;
}

/**
 * @brief Sets the string of text to display
 * @param[in] str String to display
 */
void Text::SetString(const std::string& str)
{
	m_lines.clear();

	m_lineHeight = 0;

	std::string line;
	unsigned int lineWidth = 0;
	for (char c : str)
	{
		if (c == '\n')
		{
			m_lines.emplace_back();
			m_lines.back().str = line;
			m_lines.back().width = lineWidth;

			line = "";
			lineWidth = 0;
		}
		else
		{
			line += c;
			const Font::GlyphMetrics* metrics = m_font->GetGlyphMetrics(c);
			if (metrics != nullptr)
			{
				m_lineHeight = glm::max(m_lineHeight, metrics->height);
				lineWidth += metrics->advance;
			}
		}
	}

	if (!line.empty())
	{
		m_lines.emplace_back();
		m_lines.back().str = line;
		m_lines.back().width = lineWidth;
	}

	ComputeSize(&m_width, &m_height);
}

/**
 * @brief Sets the text color
 * @param[in] color Text color
 */
void Text::SetColor(const glm::vec4& color)
{
	m_color = color;
}

/**
 * @brief Sets the position of the text
 * @param[in] x X-position
 * @param[in] y Y-position
 */
void Text::SetPosition(const float& x, const float& y)
{
	m_x = x;
	m_y = y;
}

/**
 * @brief Gets the text position
 * @return Text position
 */
glm::vec2 Text::GetPosition() const
{
	return glm::vec2(m_x, m_y);
}

/**
 * @brief Draws the text
 * @param[in] projectionMatrix Projection matrix
 */
void Text::Draw(const glm::mat4& projectionMatrix)
{
	if ((nullptr == m_font) || !m_font->IsLoaded())
	{
		return;
	}

	m_shader->Use();

	m_shader->SetUniformMatrix4fv("projection", false, glm::value_ptr(projectionMatrix));
	m_shader->SetUniform3f("textColor", m_color.r, m_color.g, m_color.b);

	glBindVertexArray(m_vao);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float x = m_x;
	float y = m_y;
	for (int j = m_lines.size() - 1; j >= 0; --j)
	{
		if (j < m_lines.size() - 1)
		{
			y += m_lineHeight + m_lineSpacing;
		}

		Line& line = m_lines[j];

		if (m_horizontalAlign == HorizontalAlignment::LEFT)
		{
			x = m_x;
		}
		else if (m_horizontalAlign == HorizontalAlignment::CENTER)
		{
			x = m_x + (m_width - line.width) / 2.0f;
		}
		else
		{
			x = m_x + m_width - line.width;
		}

		std::string& lineStr = line.str;
		for (size_t i = 0; i < lineStr.size(); ++i)
		{
			char c = lineStr[i];
			const Font::GlyphMetrics* metrics = m_font->GetGlyphMetrics(c);
			if (nullptr == metrics)
			{
				continue;
			}

			float xPos = static_cast<float>(x + metrics->bearingX);
			float yPos = static_cast<float>(y - (metrics->height - metrics->bearingY));

			unsigned int width = metrics->width;
			unsigned int height = metrics->height;

			GLfloat vertices[6][4] =
			{
				{ xPos, yPos, 0.0f, 1.0f },
				{ xPos + width, yPos, 1.0f, 1.0f },
				{ xPos + width, yPos + height, 1.0f, 0.0f },

				{ xPos + width, yPos + height, 1.0f, 0.0f },
				{ xPos, yPos + height, 0.0f, 0.0f },
				{ xPos, yPos, 0.0f, 1.0f }
			};

			glBindTexture(GL_TEXTURE_2D, m_font->GetGlyphTextureId(c));

			glActiveTexture(GL_TEXTURE0);
			m_shader->SetUniform1i("tex", 0);

			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += metrics->advance;
		}
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);

	m_shader->Unuse();
}

/**
 * @brief Computes the size of the bounds of the text
 * @param[out] Variable to set the width to
 * @param[out] Variable to set the height to
 */
void Text::ComputeSize(int* width, int* height)
{
	unsigned int w = 0, maxLineWidth = 0;

	if (m_font->IsLoaded())
	{
		for (size_t j = 0; j < m_lines.size(); ++j)
		{
			std::string& line = m_lines[j].str;
			for (size_t i = 0; i < line.size(); ++i)
			{
				char c = line[i];
				const Font::GlyphMetrics* metrics = m_font->GetGlyphMetrics(c);

				w += metrics->advance;
			}

			maxLineWidth = glm::max(maxLineWidth, w);
			w = 0;
		}
	}

	if (nullptr != width)
	{
		*width = maxLineWidth;
	}
	if (nullptr != height)
	{
		*height = m_lines.size() * m_lineHeight + m_lineSpacing * (m_lines.size() - 1);
	}
}

/**
 * Sets the horizontal alignment of the text
 * @param[in] horizontalAlign Horizontal alignment
 */
void Text::SetHorizontalAlignment(HorizontalAlignment horizontalAlign)
{
	m_horizontalAlign = horizontalAlign;
}

