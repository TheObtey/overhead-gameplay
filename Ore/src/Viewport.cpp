#include "Viewport.h"
#include "Logger.hpp"
#include "RenderGraph.h"
#include "Passes/Pass.h"

using namespace Ore;
Viewport::Viewport(uint16 x, uint16 y, uint16 width, uint16 height, Color const& backgroundColor)
{
    Setup(x, y, width, height, backgroundColor);
}

void Viewport::Setup(uint16 x, uint16 y, uint16 width, uint16 height, Color const &backgroundColor)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_backgroundColor = backgroundColor;

    m_pRenderGraph = std::make_unique<RenderGraph>(*this);
    //m_pGeometryPass = std::make_unique<GeometryPass>()
}

void Viewport::Setup(glm::uvec2 const &pos, glm::uvec2 const &size, Color const &backgroundColor)
{
    Setup(pos.x, pos.y, size.x, size.y, backgroundColor);
}

void Viewport::SetSize(uint16 width, uint16 height)
{
    m_width = width;
    m_height = height;
    m_pRenderGraph->UpdateGBuffer();
}

void Viewport::Clear() const
{
    glViewport(0, 0, m_width, m_height);
}

void Viewport::AddPass(Pass* pPass)
{
    m_pRenderGraph->AddPass(pPass);
}

void Viewport::Present() const
{
    m_pRenderGraph->Execute();
}
