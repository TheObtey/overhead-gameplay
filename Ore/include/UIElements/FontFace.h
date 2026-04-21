#ifndef ORE_FONTFACE__H_
#define ORE_FONTFACE__H_

#include "Define.h"
#include <filesystem>

namespace Ore
{
	struct FontFace
	{
	public:
		virtual ~FontFace() = default;

		virtual void SetSize(uint32 width, uint32 height) = 0;
		virtual void LoadChars() = 0;
	};
}

#endif