#pragma once

#include <glad/glad.h>

#include <vector>
#include <string>

/**
 * Font
 */
class Font
{
public:
    /**
     * Glyph metrics
     */
	struct GlyphMetrics
	{
        /**
         * Glyph width
         */
		int width;

        /**
         * Glyph height
         */
		int height;

        /**
         * Bearing (X)
         */
		int bearingX;

        /**
         * Bearing (Y)
         */
		int bearingY;

        /**
         * Advance amount
         */
		int advance;
	};

private:
	friend class Text;

    /**
     * Font size
     */
	int m_fontSize;

    /**
     * List of glyph metrics
     */
	std::vector<GlyphMetrics> m_glyphMetrics;

    /**
     * Array of texture objects for each glyph
     */
	GLuint m_glyphTextures[128];

    /**
     * Flag indicating whether the font was loaded or not
     */
	bool m_isLoaded;

    /**
     * Path to the font file
     */
	std::string m_fontFilePath;

public:

    /**
     * @brief Constructor
     */
	Font();

    /**
     * @brief Copy constructor
     * @param[in] other Font object to copy from
     */
	Font(const Font& other);

    /**
     * @brief Destructor
     */
	~Font();

    /**
     * @brief Load the font from the given filename
     * @param[in] fontFilePath Path to the font file
     * @return True if loading was successful. False otherwise
     */
	bool Load(const char* fontFilePath);

    /**
     * @brief Query whether the font has already been loaded or not.
     * @return True if the font has been loaded. False otherwise
     */
	bool IsLoaded() const;

    /**
     * @brief Sets the font size
     * @param[in] fontSize New font size
     */
	void SetSize(int fontSize);

    /**
     * @brief Gets the glyph metrics for the provided character
     * @param[in] c Character whose glyph metrics we are querying
     * @return Glyph metrics for the provided character
     */
	const GlyphMetrics* GetGlyphMetrics(char c);

private:
    /**
     * @brief Gets the texture id for the provided character
     * @param[in] c Character whose texture ID we are querying
     * @return Texture ID for the provided character
     */
	GLuint GetGlyphTextureId(char c);
};

