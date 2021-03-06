#include "Engine/Font/Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

/**
 * @brief Constructor
 */
Font::Font()
	: m_fontSize(30)
	, m_isLoaded(false)
{
	glGenTextures(128, m_glyphTextures);
}

/**
 * @brief Copy constructor
 * @param[in] other Font object to copy from
 */
Font::Font(const Font& other)
{
	m_fontSize = other.m_fontSize;
	m_glyphMetrics = other.m_glyphMetrics;

	for (size_t i = 0; i < 128; ++i)
	{
		m_glyphTextures[i] = other.m_glyphTextures[i];
	}

	m_isLoaded = other.m_isLoaded;

	m_fontFilePath = other.m_fontFilePath;
}

/**
 * @brief Destructor
 */
Font::~Font()
{
	if (nullptr != m_glyphTextures)
	{
		glDeleteTextures(128, m_glyphTextures);
	}
}

/**
 * @brief Load the font from the given filename
 * @param[in] fontFilePath Path to the font file
 * @return True if loading was successful. False otherwise
 */
bool Font::Load(const char* fontFilePath)
{
	// Reference: https://learnopengl.com/In-Practice/Text-Rendering

	m_isLoaded = false;

	FT_Library ftLibray;
	FT_Face ftFace;

	// FT libraries return 0 on success
	if (FT_Init_FreeType(&ftLibray))
	{
		std::printf("Failed to load freetype library!\n");
		return false;
	}
	if (FT_New_Face(ftLibray, fontFilePath, 0, &ftFace))
	{
		std::printf("Failed to load font! %s\n", fontFilePath);
		return false;
	}

	m_fontFilePath = std::string(fontFilePath);

	m_glyphMetrics.clear();

	FT_Set_Pixel_Sizes(ftFace, 0, m_fontSize);

	for (int c = 0; c < 128; ++c)
	{
		if (FT_Load_Char(ftFace, c, FT_LOAD_RENDER))
		{
			std::printf("Failed to load char %c\n", static_cast<char>(c));
			continue;
		}

		glBindTexture(GL_TEXTURE_2D, m_glyphTextures[c]);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
			ftFace->glyph->bitmap.width,
			ftFace->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, ftFace->glyph->bitmap.buffer);

		GlyphMetrics metrics =
		{
			static_cast<int>(ftFace->glyph->bitmap.width),
			static_cast<int>(ftFace->glyph->bitmap.rows),

			ftFace->glyph->bitmap_left, 
			ftFace->glyph->bitmap_top,

			static_cast<int>(ftFace->glyph->advance.x >> 6)
		};
		m_glyphMetrics.push_back(metrics);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(ftFace);
	FT_Done_FreeType(ftLibray);

	m_isLoaded = true;

	return true;
}

/**
 * @brief Query whether the font has already been loaded or not.
 * @return True if the font has been loaded. False otherwise
 */
bool Font::IsLoaded() const
{
	return m_isLoaded;
}

/**
 * @brief Sets the font size
 * @param[in] fontSize New font size
 */
void Font::SetSize(int fontSize)
{
	if (!m_isLoaded)
	{
		return;
	}

	if (m_fontSize != fontSize)
	{
		m_fontSize = fontSize;
		Load(m_fontFilePath.c_str());
	}
}

/**
 * @brief Gets the glyph metrics for the provided character
 * @param[in] c Character whose glyph metrics we are querying
 * @return Glyph metrics for the provided character
 */
const Font::GlyphMetrics* Font::GetGlyphMetrics(char c)
{
	return &m_glyphMetrics[c];
}

/**
 * @brief Gets the texture id for the provided character
 * @param[in] c Character whose texture ID we are querying
 * @return Texture ID for the provided character
 */
GLuint Font::GetGlyphTextureId(char c)
{
	return m_glyphTextures[c];
}

