#ifndef GRAPHIC_SERVER__H_
#define GRAPHIC_SERVER__H_

#include "Geometry.h"
#include "Server.hpp"
#include "Shader.h"
#include "Texture.h"
#include "Program.h"

class NodeCamera;
class NodeWindow;
class NodeViewport;
class Window;

template <>
struct Command<class GraphicServer>
{
    enum class CmdType
    {
        LOADSHADERS, SETUPVIEWPORT, ATTACHCAMERA, ATTACHVIEWPORT, OPENWINDOW, CLEAR, PRESENT, BEGINFRAME, ENDFRAME
    } Type;

    NodeWindow* pNodeWindow = nullptr;
    NodeViewport* pNodeViewport = nullptr;
    NodeCamera* pNodeCamera = nullptr;
};

class GraphicServer : public Server<GraphicServer>
{
public:
    using CommandType = Command<GraphicServer>::CmdType;

    static void AttachToWindow(NodeViewport* pViewport, NodeWindow* pWindow);
    static void AttachToViewport(NodeCamera* pCamera, NodeViewport* pViewport);
    static void OpenWindow(NodeWindow* pWindow);
    static void Present(NodeViewport* pViewport);
    static void Clear(NodeViewport* pViewport);
    static void BeginFrame(NodeWindow* pWindow);
    static void EndFrame(NodeWindow* pWindow);
    static void LoadShaderPrograms(NodeViewport* pViewport);

    static Ore::Shader& GetGeoVert() { return Instance().m_geoVert; }
    static Ore::Shader& GetGeoFrag() { return Instance().m_geoFrag; }
    static Ore::Program& GetGeoProgram() { return Instance().m_geoProgram; }
    static Ore::Shader& GetLightVert() { return Instance().m_lightVert; }
    static Ore::Shader& GetLightFrag() { return Instance().m_lightFrag; }
    static Ore::Program& GetLightProgram() { return Instance().m_lightProgram; }
    static Ore::Shader& GetAnimatedVert() { return Instance().m_animatedVert; }
    static Ore::Program& GetAnimatedProgram() { return Instance().m_animatedProgram; }

    static sptr<Ore::Geometry> GetDefaultGeo() { return Instance().m_defaultCubeGeo; }
    static sptr<Ore::Texture> GetDefaultTexture() { return Instance().m_defaultTexture; }

private:
    void FlushCommandsImpl() override;
    void BuildTasksImpl(TaskGraph& graph) override;
	void OnInitialize() override;
    void OnUnInitialize() override;
    void LoadShader();

private:
    //default shaders
    Ore::Program m_geoProgram;
    Ore::Program m_lightProgram;
    Ore::Program m_animatedProgram;
    Ore::Shader m_animatedVert { Ore::ShaderType::TYPE_VERTEX};

    Ore::Shader m_geoVert{ Ore::ShaderType::TYPE_VERTEX };
    Ore::Shader m_geoFrag { Ore::ShaderType::TYPE_FRAGMENT};

    Ore::Shader m_lightVert{ Ore::ShaderType::TYPE_VERTEX };
    Ore::Shader m_lightFrag { Ore::ShaderType::TYPE_FRAGMENT};

    sptr<Ore::Geometry> m_defaultCubeGeo;
    sptr<Ore::Texture> m_defaultTexture;

    std::vector<NodeWindow*> m_pWindowsToSwap;
};

#endif
