#include "RenderGraph.h"
#include "TextureObject.h"
#include "Passes/Pass.h"

#include <Logger.hpp>

RenderGraph::RenderGraph(uint32 screenWidth, uint32 screenHeight)
{
    RenderGraph::CreateGBuffer(screenWidth, screenHeight);
}

RenderGraph::~RenderGraph()
{
}

void RenderGraph::SetSize(uint32 width, uint32 height)
{
    for (Pass* pPass : m_passes)
        pPass->SetSize(width, height);
}

void RenderGraph::CreateGBuffer(uint32 screenWidth, uint32 screenHeight)
{
    Logger::Log("Start G-Buffer");
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    glGenFramebuffers(1, &m_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

    uint32 gPosition, gNormal, gAlbedoSpec;
    glGenTextures(1, &gPosition);
    m_pGPosition = std::make_shared<TextureObject>(gPosition, TextureType::TYPE_2D);
    m_pGPosition->Bind();

    m_pGPosition->GenerateTexture(DataType::FLOAT, screenWidth, screenHeight, GL_RGBA16F);  
    m_pGPosition->AddParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_pGPosition->AddParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_pGPosition->AttachToFrameBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0);

    glGenTextures(1, &gNormal);
    m_pGNormal = std::make_shared<TextureObject>(gNormal, TextureType::TYPE_2D);
    m_pGNormal->Bind();

    m_pGNormal->GenerateTexture(DataType::FLOAT, screenWidth, screenHeight, GL_RGBA16F);  
    m_pGNormal->AddParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_pGNormal->AddParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_pGNormal->AttachToFrameBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1);

    glGenTextures(1, &gAlbedoSpec);
    m_pGAlbedoSpec = std::make_shared<TextureObject>(gAlbedoSpec, TextureType::TYPE_2D);
    m_pGAlbedoSpec->Bind();

    m_pGAlbedoSpec->GenerateTexture(DataType::UBYTE, screenWidth, screenHeight, GL_RGBA);  
    m_pGAlbedoSpec->AddParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_pGAlbedoSpec->AddParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_pGAlbedoSpec->AttachToFrameBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2);

    uint32 attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    uint32 rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Logger::LogWithLevel(LogLevel::ERROR, "Framebuffer not complete !");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderGraph::AddPass(Pass* pPass)
{
    pPass->SetGBuffer(m_gBuffer);
    pPass->SetTextures(m_pGPosition, m_pGNormal, m_pGAlbedoSpec);
    pPass->SetScreenSize(m_screenWidth, m_screenHeight);
    m_passes.push_back(pPass);
}

void RenderGraph::Execute()
{
    for(Pass* pPass : m_passes)
    {
        pPass->Execute();
    }
}
