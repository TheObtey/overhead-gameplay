#include "Mesh.h"
#include "IProgram.h"

#include <Logger.hpp>

Mesh::Mesh(sptr<Geometry> const& geometry, TextureSpan textures, glm::mat4 const& transform)
{
    Logger::Log("Start Mesh");
    m_pGeometry = geometry;
    m_transform = transform;
    m_textures = textures;
}

void Mesh::Draw(IProgram const& pProgram) const
{
    Logger::Log("Start Draw Mesh");
    uint32 diffuseNr = 1;
    uint32 specularNr = 1;
    uint32 normalNr = 1;
    uint32 heightNr = 1;

    for(uint32 i = 0; i<m_textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i); 
        std::string name;
        TextureMaterialType matType = m_textures[i].get().GetTextureMaterialType();
        switch(matType)
        {
        case(TextureMaterialType::DIFFUSE):
            name = "texture_diffuse" + std::to_string(diffuseNr++);
            break;
        case(TextureMaterialType::SPECULAR):
            name = "texture_specular" + std::to_string(specularNr++);
            break;
        case(TextureMaterialType::NORMAL):
            name = "texture_normal" + std::to_string(normalNr++);
            break;
        case(TextureMaterialType::HEIGHT):
            name = "texture_height" + std::to_string(heightNr++);
            break;
        }

        Logger::Log(name);
        glUniform1i(glGetUniformLocation(pProgram.GetProgramId(), name.c_str()), i);
        m_textures[i].get().GetTextureObject().Bind();
    }

    m_pGeometry->Draw();
    glActiveTexture(GL_TEXTURE0);
}
