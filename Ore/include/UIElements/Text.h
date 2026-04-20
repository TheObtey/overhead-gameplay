#ifndef ORE_TEXT__H_
#define ORE_TEXT__H_

#include "UIElement.h"

#include <filesystem>

namespace Ore
{
struct Text : public UIElement
{
    Text() = default;
    Text(std::string _text, std::filesystem::path _fontPath, uint32 _fontSize)
    {
        text = _text, fontPath = _fontPath, fontSize = _fontSize;
    }

    std::string text;
    std::filesystem::path fontPath;
    uint32 fontSize;
};
}
#endif
