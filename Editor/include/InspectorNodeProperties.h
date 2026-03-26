#ifndef EDITOR_INSPECTOR_NODE_PROPERTIES_H__
#define EDITOR_INSPECTOR_NODE_PROPERTIES_H__

#include <Serialization/json.hpp>

#include <imgui.h>
#include <imfilebrowser.h>

#include <string>
#include <map>

class Node;
class EditorImGui;

using json = nlohmann::json;

class InspectorNodeProperties
{
public:

    InspectorNodeProperties(EditorImGui* pImGuiEditor);


    void DrawWindow(bool windowState, Node* pSelectedNode);

    void SetWindow(int width, int height) { m_screenWidth = width; m_screenHeight = height; }

    void SetDirty() { m_isDirty = true; }
private:
    bool DrawDatas(json& publicDataJson);
    bool DrawLuaScriptPicker(json& publicDataJson);

private:
    EditorImGui* m_pImguiEditor = nullptr;

    int m_screenWidth = 0;
    int m_screenHeight = 0;

    int m_windowWidth = 0;
    int m_windowHeight = 0;

    bool m_isOpen = false;
    bool m_isDirty = false;

    Node* m_pSelectedNode = nullptr;

    json m_currentDatas;

    bool m_showLuaBrowser = false;
    ImGui::FileBrowser m_luaBrowser;
    int m_fileBrowsingSizeX = 800;
    int m_fileBrowsingSizeY = 450;

};

#endif //!EDITOR_INSPECTOR_NODE_PROPERTIES_H__