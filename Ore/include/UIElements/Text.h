#ifndef ORE_TEXT__H_
#define ORE_TEXT__H_

#include "UIElement.h"
#include "UIElements/FontFace.h"

#include <filesystem>

namespace Ore
{
class Text : public UIElement
{
public:
    Text() = default;
    Text(std::string const& _text, sptr<FontFace>& _face) : m_text(_text), m_pFontFace(_face) {}
    ~Text() = default;

    void Draw(Program& program, int32 &index) override;

private:
    std::string m_text;
    sptr<FontFace> m_pFontFace;
};
}
#endif
