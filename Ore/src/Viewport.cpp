#include "Viewport.h"
#include "Logger.hpp"
#include "RenderGraph.h"
#include "Passes/Pass.h"

Viewport::Viewport(uint16 x, uint16 y, uint16 width, uint16 height, Color const& backgroundColor)
{
    Setup(x, y, width, height, backgroundColor);
}

void Viewport::AddPass(Pass* pPass)
{
    m_pRenderGraph->AddPass(pPass);
}

void Viewport::Setup(uint16 x, uint16 y, uint16 width, uint16 height, Color const &backgroundColor)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_backgroundColor = backgroundColor;
    m_pRenderGraph = std::make_unique<RenderGraph>(width, height);
}

void Viewport::Setup(glm::uvec2 const &pos, glm::uvec2 const &size, Color const &backgroundColor)
{
    Setup(pos.x, pos.y, size.x, size.y, backgroundColor);
}

void Viewport::SetSize(uint16 width, uint16 height)
{
    m_width = width;
    m_height = height;
    m_pRenderGraph->SetSize(width, height);
}

void Viewport::Clear() const
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(m_x, m_y, m_width, m_height);
    glClearColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

void Viewport::Present() const
{
    m_pRenderGraph->Execute();
}
