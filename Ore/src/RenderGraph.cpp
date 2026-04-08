#include "RenderGraph.h"
#include "TextureObject.h"
#include "Passes/Pass.h"

#include <Logger.hpp>

#include "Viewport.h"

using namespace Ore;
RenderGraph::RenderGraph(Viewport& viewport) : m_viewport(viewport)
{
    glGenFramebuffers(1, &m_gBuffer);

    uint32 gPosition, gNormal, gAlbedoSpec;
    glGenTextures(1, &gPosition);
    m_pGPosition = std::make_shared<TextureObject>(gPosition, TextureType::TYPE_2D);

    glGenTextures(1, &gNormal);
    m_pGNormal = std::make_shared<TextureObject>(gNormal, TextureType::TYPE_2D);

    glGenTextures(1, &gAlbedoSpec);
    m_pGAlbedoSpec = std::make_shared<TextureObject>(gAlbedoSpec, TextureType::TYPE_2D);

    glGenRenderbuffers(1, &m_rboDepth);
    RenderGraph::CreateGBuffer(viewport.GetWidth(), viewport.GetHeight());
}

void RenderGraph::UpdateGBuffer()
{
    CreateGBuffer(m_viewport.GetWidth(), m_viewport.GetHeight());
    for (Pass* pPass : m_passes)
    {
        pPass->SetScreenSize(m_viewport.GetWidth(), m_viewport.GetHeight());
        pPass->SetGBuffer(m_gBuffer);
    }
}

void RenderGraph::CreateGBuffer(uint32 const screenWidth, uint32 const screenHeight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

    m_pGPosition->Bind();

    m_pGPosition->GenerateTexture(DataType::FLOAT, screenWidth, screenHeight, GL_RGBA16F);  
    m_pGPosition->AddParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_pGPosition->AddParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_pGPosition->AttachToFrameBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0);

    m_pGNormal->Bind();

    m_pGNormal->GenerateTexture(DataType::FLOAT, screenWidth, screenHeight, GL_RGBA16F);  
    m_pGNormal->AddParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_pGNormal->AddParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_pGNormal->AttachToFrameBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1);

    m_pGAlbedoSpec->Bind();

    m_pGAlbedoSpec->GenerateTexture(DataType::UBYTE, screenWidth, screenHeight, GL_RGBA);  
    m_pGAlbedoSpec->AddParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_pGAlbedoSpec->AddParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_pGAlbedoSpec->AttachToFrameBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2);

    uint32 attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Logger::LogWithLevel(LogLevel::ERROR, "Framebuffer not complete !");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderGraph::AddPass(Pass* pPass)
{
    pPass->SetGBuffer(m_gBuffer);
    pPass->SetTextures(m_pGPosition, m_pGNormal, m_pGAlbedoSpec);
    pPass->SetScreenSize(m_viewport.GetWidth(), m_viewport.GetHeight());
    m_passes.push_back(pPass);
}

void RenderGraph::Execute()
{
    //TODO Scissor for background color

    for(Pass* pPass : m_passes)
        pPass->Execute();

   End();
}

void RenderGraph::End()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(0,0, m_viewport.GetWidth(), m_viewport.GetHeight(),
        m_viewport.GetPosX(), m_viewport.GetPosY(), m_viewport.GetPosX() + m_viewport.GetWidth(), m_viewport.GetPosY() + m_viewport.GetHeight(),
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}
