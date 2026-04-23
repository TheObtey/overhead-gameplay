#include "UIElements/FTFontFace.h"
#include "Logger.hpp"
#include "TextureObject.h"

#include <glad/glad.h>
#include <filesystem>
#include <span>

namespace Ore
{
FT_Library FTFontFace::s_ftLibrary;
int32 FTFontFace::s_refcount = 0;

FTFontFace::FTFontFace(std::filesystem::path const& path, uint32 size)
{
	if (s_refcount == 0 && FT_Init_FreeType(&s_ftLibrary))
	{
		Logger::LogWithLevel(LogLevel::ERROR, "ERROR::FREETYPE: Could not init FreeType Library");
		return;
	}

	s_refcount++;

	if (FT_New_Face(s_ftLibrary, path.string().c_str(), 0, &m_face))
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Failed to load font");
		return;
	}

	FTFontFace::SetSize(0, size);
	FTFontFace::LoadChars();
	Logger::LogWithLevel(LogLevel::DEBUG, "Created font atlas for : ", path);
}

FTFontFace::FTFontFace(FTFontFace const& other)
{
	m_face = other.m_face;
	m_bitmap = other.m_bitmap;
	m_texture = other.m_texture;
	s_refcount++;
}

FTFontFace::FTFontFace(FTFontFace&& other) noexcept
{
	m_face = other.m_face;
	m_bitmap = other.m_bitmap;
	m_texture = other.m_texture;
	m_face = other.m_face;
}

void FTFontFace::SetSize(uint32 const width, uint32 const height)
{
	FT_Set_Pixel_Sizes(m_face, width, height);
}

void FTFontFace::GenerateAtlasTexture()
{
	glGenTextures(1, &m_bitmap);

	m_texture = TextureObject(m_bitmap, TextureType::TYPE_2D);
	m_texture.Bind();
	m_texture.GenerateTexture(DataType::UBYTE, BitmapSize, BitmapSize, GL_RED, GL_RED);
	m_texture.AddParameters(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_texture.AddParameters(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_texture.AddParameters(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	m_texture.AddParameters(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glClearTexImage(m_bitmap, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
}

void FTFontFace::LoadChars()
{
	GenerateAtlasTexture();

	int32 advanceX = 0;
	int32 advanceY = 0;
	int32 maxGlyphHeightRow = 0;
	for (uint8 c = 32; c < 128; c++)
	{
		if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
		{
			Logger::LogWithLevel(LogLevel::ERROR, "ERROR::FREETYTPE: Failed to load Glyph");
			return;
		}

		int32 const glyphHeight = static_cast<int32>(m_face->glyph->bitmap.rows);
		int32 const glyphWidth =  static_cast<int32>(m_face->glyph->bitmap.width);

		if (advanceX + glyphWidth >= BitmapSize)
		{
			advanceX = 0;
			advanceY += maxGlyphHeightRow;
			maxGlyphHeightRow = 0;
		}

		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			advanceX,
			advanceY,
			glyphWidth,
			glyphHeight,
			GL_RED,
			GL_UNSIGNED_BYTE,
			m_face->glyph->bitmap.buffer
		);
		maxGlyphHeightRow = std::max(glyphHeight, maxGlyphHeightRow);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)

		Character character = {
	 		m_bitmap,
			{advanceX / static_cast<float>(BitmapSize), advanceY / static_cast<float>(BitmapSize)},
			{ glyphWidth, glyphHeight},
			{m_face->glyph->bitmap_left, m_face->glyph->bitmap_top},
			static_cast<uint32>(m_face->glyph->advance.x >> 6)
		};

		advanceX += (m_face->glyph->advance.x >> 6) + 3;  // bitshift by 6 to get value in pixels (2^6 = 64)
		m_loadedCharacters[c] = character;
	}
	FT_Done_Face(m_face);
}

FTFontFace::~FTFontFace()
{
	s_refcount--;
	if (s_refcount == 0) FT_Done_FreeType(s_ftLibrary);
}
};
