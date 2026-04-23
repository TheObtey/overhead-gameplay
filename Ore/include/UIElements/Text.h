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

    void Draw(Program& program, int32 &index) const override
    {
        glActiveTexture(GL_TEXTURE0 + index);
        program.SetUniform("textures["+std::to_string(index)+"]", index);
        glBindTexture(GL_TEXTURE_2D, m_pFontFace->m_bitmap);
        int32 textureIndex = index;
        int32 currentOffset = 0;

        for (char const c : m_text)
        {
            std::string strIndex = std::to_string(index);
            Character const& character = m_pFontFace->GetCharacter(c);

            glm::vec2 uvs[4] = {
             {character.uvPos.x, character.uvPos.y},
             {character.uvPos.x + character.charSize.x / 1024.0f, character.uvPos.y},
             {character.uvPos.x + character.charSize.x, character.uvPos.y + character.charSize.y},
             {character.uvPos.x, character.uvPos.y + character.charSize.y},
            };

            glm::vec2 pos = {x + character.charBearing.x + currentOffset , y - (character.charSize.y - character.charBearing.y)};
            currentOffset += character.advance;
            //size
            //texture

            program.SetUniform("uvPos["+strIndex+"][0]", uvs[0]);
            program.SetUniform("uvPos["+strIndex+"][1]", uvs[1]);
            program.SetUniform("uvPos["+strIndex+"][2]", uvs[2]);
            program.SetUniform("uvPos["+strIndex+"][3]", uvs[3]);

            program.SetUniform("offsets["+strIndex+"]", pos);
            program.SetUniform("sizes["+strIndex+"]", glm::vec2(character.charSize.x, character.charSize.y));
            program.SetUniform("textureId["+strIndex+"]", textureIndex);

            index++;
        }
    }

private:
    std::string m_text;
    sptr<FontFace> m_pFontFace;
};
}
#endif
