#ifndef ORE_FONTFACE__H_
#define ORE_FONTFACE__H_

#include <unordered_map>

#include "Define.h"
#include <glm/vec2.hpp>

namespace Ore
{
	struct Character
	{
		uint32 atlasId;
		glm::ivec2 charSize;
		glm::ivec2 charBearing;
		uint32 advance; //Offset to the next glyph
	};

	struct FontFace
	{
	public:
		virtual ~FontFace() = default;

		virtual void SetSize(uint32 width, uint32 height) = 0;
		virtual void LoadChars() = 0;
		Character const& GetCharacter(char const c) const { return m_loadedCharacters.at(c); }

	protected:
		uint32 m_bitmap = 0;
		std::unordered_map<char, Character> m_loadedCharacters {};
	};
}

#endif