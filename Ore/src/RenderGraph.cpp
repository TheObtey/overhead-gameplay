#include "RenderGraph.h"
#include "TextureObject.h"
#include "Logger.hpp"
#include "Passes/Pass.h"

#include <glad/glad.h>

RenderGraph::RenderGraph(uint32 screenWidth, uint32 screenHeight)
{
    CreateGBuffer(screenWidth, screenHeight);
}

RenderGraph::~RenderGraph()
{
}

void RenderGraph::CreateGBuffer(uint32 screenWidth, uint32 screenHeight)
{
    Logger::Log("Start G-Buffer");
    glGenFramebuffers(1, &m_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

    uint32 gPosition, gNormal, gAlbedo;
    glGenTextures(1, &gPosition);
    TextureObject position(gPosition, TextureType::TYPE_2D);
    position.Bind();

    position.GenerateTexture(DataType::FLOAT, screenWidth, screenHeight, GL_RGBA16F);  
    position.AddParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    position.AddParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    position.AttachToFrameBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0);

    glGenTextures(1, &gNormal);
    TextureObject normal(gNormal, TextureType::TYPE_2D);
    normal.Bind();

    normal.GenerateTexture(DataType::FLOAT, screenWidth, screenHeight, GL_RGBA16F);  
    normal.AddParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    normal.AddParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    normal.AttachToFrameBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1);

    glGenTextures(1, &gAlbedo);
    TextureObject albedo(gAlbedo, TextureType::TYPE_2D);
    albedo.Bind();

    albedo.GenerateTexture(DataType::UBYTE, screenWidth, screenHeight, GL_RGBA);  
    albedo.AddParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    albedo.AddParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    albedo.AttachToFrameBuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2);

    uint32 attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    uint32 rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

   if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Logger::LogWithLevel(LogLevel::ERROR, "Framebuffer not complete !");

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderGraph::AddPass(Pass& pass)
{
    pass.SetGBuffer(m_gBuffer);
    m_passes.push_back(std::make_unique<Pass>(pass));
}

void RenderGraph::Execute()
{
    for(auto const& pass : m_passes)
    {
        pass->Execute();
    }
}
