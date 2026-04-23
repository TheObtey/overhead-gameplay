#include "UIElements/Text.h"

namespace Ore
{
    void Text::Draw(Program &program, int32 &index) const
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
                {character.uvPos.x, character.uvPos.y + character.charSize.y / 1024.0f},
                {character.uvPos.x + character.charSize.x / 1024.0f, character.uvPos.y + character.charSize.y / 1024.0f},
                {character.uvPos.x + character.charSize.x / 1024.0f, character.uvPos.y},
                {character.uvPos.x, character.uvPos.y},
               };

            glm::vec2 pos = {x + character.charBearing.x * scale + currentOffset , y - character.charBearing.y  * scale};
            currentOffset += character.advance * scale;
            //size
            //texture

            program.SetUniform("uvPos["+strIndex+"][0]", uvs[0]);
            program.SetUniform("uvPos["+strIndex+"][1]", uvs[1]);
            program.SetUniform("uvPos["+strIndex+"][2]", uvs[2]);
            program.SetUniform("uvPos["+strIndex+"][3]", uvs[3]);

            program.SetUniform("offsets["+strIndex+"]", pos);
            program.SetUniform("sizes["+strIndex+"]", glm::vec2(character.charSize.x, character.charSize.y) * scale);
            program.SetUniform("textureId["+strIndex+"]", textureIndex);
            program.SetUniform("isText["+strIndex+"]", true);
            program.SetUniform("tint["+strIndex+"]", color);

            index++;
        }
    }
}

