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

    static Shader& GetGeoVert() { return Instance().m_geoVert; }
    static Shader& GetGeoFrag() { return Instance().m_geoFrag; }
    static Program& GetGeoProgram() { return Instance().m_geoProgram; }
    static Shader& GetLightVert() { return Instance().m_lightVert; }
    static Shader& GetLightFrag() { return Instance().m_lightFrag; }
    static Program& GetLightProgram() { return Instance().m_lightProgram; }

    static sptr<Geometry> GetDefaultGeo() { return Instance().m_defaultCubeGeo; }
    static sptr<Texture> GetDefaultTexture() { return Instance().m_defaultTexture; }

private:
    void FlushCommandsImpl() override;
    void BuildTasksImpl(TaskGraph& graph) override;
	void OnInitialize() override;
    void OnUnInitialize() override;
    void LoadShader();

private:
    //default shaders
    Program m_geoProgram;
    Program m_lightProgram;

    Shader m_geoVert {ShaderType::TYPE_VERTEX};
    Shader m_geoFrag {ShaderType::TYPE_FRAGMENT};

    Shader m_lightVert {ShaderType::TYPE_VERTEX};
    Shader m_lightFrag {ShaderType::TYPE_FRAGMENT};

    sptr<Geometry> m_defaultCubeGeo;
    sptr<Texture> m_defaultTexture;

    std::vector<NodeWindow*> m_pWindowsToSwap;
};

#endif
