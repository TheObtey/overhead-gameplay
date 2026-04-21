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

	FTFontFace::FTFontFace(std::filesystem::path path)
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

		FTFontFace::SetSize(0, 40);
		FTFontFace::LoadChars();
		Logger::LogWithLevel(LogLevel::DEBUG, "Created font atlas for : ", path);
	}

	FTFontFace::FTFontFace(FTFontFace const& other)
	{
		m_face = other.m_face;
		s_refcount++;
	}

	FTFontFace::FTFontFace(FTFontFace&& other) noexcept
	{
		m_face = std::move(other.m_face);
	}

	void FTFontFace::SetSize(uint32 width, uint32 height)
	{
		FT_Set_Pixel_Sizes(m_face, width, height);
	}

	void FTFontFace::LoadChars()
	{
		//generate a 1024*1024 textures
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

		int32 advanceX = 0;
		int32 advanceY = 0;
		int32 maxGlyphHeight = 0;
		for (uint8 c = 32; c < 128; c++)
		{
			if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
			{
				Logger::LogWithLevel(LogLevel::ERROR, "ERROR::FREETYTPE: Failed to load Glyph");
				return;
			}

			if (advanceX + m_face->glyph->advance.x >= BitmapSize)
			{
				advanceX = 0;
				advanceY += maxGlyphHeight;
				maxGlyphHeight = 0;
			}

			int32 glyphHeight = m_face->glyph->bitmap.rows;
			glTexSubImage2D(
				GL_TEXTURE_2D,
				0,
				advanceX,
				advanceY,
				 m_face->glyph->bitmap.width,
				glyphHeight,
				GL_RED,
				GL_UNSIGNED_BYTE,
				m_face->glyph->bitmap.buffer
			);

			if (glyphHeight > maxGlyphHeight) maxGlyphHeight = glyphHeight;
			//maxGlyphHeight = std::max(glyphHeight, maxGlyphHeight);

			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			advanceX += m_face->glyph->advance.x >> 6;  // bitshift by 6 to get value in pixels (2^6 = 64)
		}

		//GLuint buffID;
		//glGenBuffers(1, &buffID);

		//std::span<byte> charData = std::span<byte>(m_face->glyph->bitmap.buffer, m_face->glyph->bitmap.width * m_face->glyph->bitmap.rows);
		//Buffer<byte> buffer(charData, buffID, BufferType::PIXEL_UNPACK_BUFFER, false);
		//buffer.Bind();

		//buffer.
	}

	FTFontFace::~FTFontFace()
	{
		FT_Done_Face(m_face);
		s_refcount--;
		if (s_refcount == 0) FT_Done_FreeType(s_ftLibrary);
	}
};