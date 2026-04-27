#ifndef ORE_TEXT__H_
#define ORE_TEXT__H_

#include "UIElement.h"
#include "UIElements/FontFace.h"

#include <filesystem>
#include <utility>

namespace Ore
{
class Text : public UIElement
{
public:
    Text() = default;
    Text(std::string _text, sptr<FontFace>& _face) : m_text(std::move(_text)), m_pFontFace(_face) {}
    ~Text() = default;

    void Draw(Program& program, int32 &index) const override;

private:
    std::string m_text;
    sptr<FontFace> m_pFontFace;
};
}
#endif
