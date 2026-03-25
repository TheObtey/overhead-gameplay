#include "Mesh.h"
#include "Shader.h"
#include "Logger.hpp"

Mesh::Mesh(Geometry const& geometry, std::vector<Texture*> const& textures, glm::mat4 const& transform)
{
    Logger::Log("Start Mesh");
    m_pGeometry = std::make_shared<Geometry>(geometry);
    m_transform = transform;

    m_textures = textures;
}

Mesh::~Mesh()
{
}

void Mesh::Draw(sptr<IShader> const shader)
{
    Logger::Log("Start Draw Mesh");
    uint32 diffuseNr = 1;
    uint32 specularNr = 1;
    uint32 normalNr = 1;
    uint32 heightNr = 1;

    sptr<Shader> pShader = std::dynamic_pointer_cast<Shader>(shader);
    for(uint32 i = 0; i<m_textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i); 
        std::string name;
        TextureMaterialType matType = m_textures[i]->GetTextureMaterialType();
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
        glUniform1i(glGetUniformLocation(pShader->GetProgramId(), name.c_str()), i);
        m_textures[i]->GetTextureObject().Bind();
    }

    m_pGeometry->Draw(pShader);
    glActiveTexture(GL_TEXTURE0);
}
