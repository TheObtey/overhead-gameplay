#include "UIElements/Text.h"

namespace Ore
{
    void Text::Draw(Program &program, int32 &index)
    {
        glActiveTexture(GL_TEXTURE0 + index);
        program.SetUniform("textures["+std::to_string(index)+"]", index);
        glBindTexture(GL_TEXTURE_2D, m_pFontFace->m_bitmap);
        int32 textureIndex = index;

        for (char const c : m_text)
        {
            std::string strIndex = std::to_string(index);
            Character const& character = m_pFontFace->GetCharacter(c);

            glm::vec2 uvs[4] = {
             {character.uvPos.x, character.uvPos.y + character.charSize.y},
             {character.uvPos.x + character.charSize.x, character.uvPos.y + character.charSize.y},
             {character.uvPos.x + character.charSize.x, character.uvPos.y},
             {character.uvPos.x, character.uvPos.y}
            };

            glm::vec2 pos = {x + character.charBearing.x , y - (character.charSize.y - character.charBearing.y)};
            //size
            //texture

            program.SetUniform("uvPos["+strIndex+"][0]", uvs[0]);
            program.SetUniform("uvPos["+strIndex+"][1]", uvs[1]);
            program.SetUniform("uvPos["+strIndex+"][2]", uvs[2]);
            program.SetUniform("uvPos["+strIndex+"][3]", uvs[3]);

            program.SetUniform("offsets["+strIndex+"]", pos);
            program.SetUniform("sizes["+strIndex+"]", character.charSize);
            program.SetUniform("textureId["+strIndex+"]", textureIndex);

            index++;
        }
    }
}

