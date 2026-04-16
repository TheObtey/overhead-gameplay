#include "InspectorNodeProperties.h"
#include "EditorImGui.h"
#include "EditorRaylib3D.h"

#include <Node.h>
#include <Serialization/SerializeObject.hpp>
#include <iostream>
#include <imgui.h>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <cstring>

namespace
{

    std::filesystem::path const kScriptStockDir = "../Game/ScriptStock";
    std::filesystem::path const kTextureRootDir = "../Game/res/textures";
    std::filesystem::path const kScriptStockLogicalDir = "ScriptStock";

    std::string const kDefaultTexturePath = "res/textures/Default.png";

    std::string NormalizeScriptPathForProject(std::filesystem::path const& inputPath)
    {
        if (inputPath.empty())
            return {};

        std::string const in = inputPath.generic_string();

        if (in.rfind("ScriptStock/", 0) == 0)
            return in;

        if (in.rfind("../Game/ScriptStock/", 0) == 0)
            return in.substr(std::string("../Game/").size());

        std::error_code ec;
        std::filesystem::path stockAbs = std::filesystem::weakly_canonical(std::filesystem::absolute(kScriptStockDir), ec);
        if (ec)
        {
            ec.clear();
            stockAbs = std::filesystem::absolute(kScriptStockDir);
        }

        std::filesystem::path candidate = inputPath;
        if (candidate.is_relative())
            candidate = std::filesystem::absolute(candidate);

        std::filesystem::path candidateAbs = std::filesystem::weakly_canonical(candidate, ec);
        if (ec)
        {
            ec.clear();
            candidateAbs = candidate;
        }

        // Si le fichier est dans ScriptStock, on garde la sous-arborescence
        std::filesystem::path relToStock = std::filesystem::relative(candidateAbs, stockAbs, ec);
        if (!ec && !relToStock.empty())
        {
            std::string const relStr = relToStock.generic_string();
            if (!(relStr == ".." || relStr.rfind("../", 0) == 0))
            {
                return (kScriptStockLogicalDir / relToStock).generic_string();
            }
        }

        return (kScriptStockDir / candidateAbs.filename()).generic_string();
    }

    std::string NormalizeTexturePathForProject(std::filesystem::path const& inputPath)
    {
        if (inputPath.empty())
            return kDefaultTexturePath;

        std::error_code ec;

        std::filesystem::path textureRootAbs = std::filesystem::weakly_canonical(std::filesystem::absolute(kTextureRootDir), ec);
        if (ec)
        {
            ec.clear();
            textureRootAbs = std::filesystem::absolute(kTextureRootDir);
        }

        std::filesystem::path candidate = inputPath;
        if (candidate.is_relative())
            candidate = std::filesystem::absolute(candidate);

        std::filesystem::path candidateAbs = std::filesystem::weakly_canonical(candidate, ec);
        if (ec)
        {
            ec.clear();
            candidateAbs = candidate;
        }

        std::filesystem::path relToTextureRoot = std::filesystem::relative(candidateAbs, textureRootAbs, ec);
        if (!ec && !relToTextureRoot.empty())
        {
            std::string const relStr = relToTextureRoot.generic_string();
            if (!(relStr == ".." || relStr.rfind("../", 0) == 0))
            {
                return (std::filesystem::path("res/textures") / relToTextureRoot).generic_string();
            }
        }

        return (std::filesystem::path("res/textures") / candidateAbs.filename()).generic_string();
    }
}

InspectorNodeProperties::InspectorNodeProperties(EditorImGui* pImGuiEditor)
    : m_pImguiEditor(pImGuiEditor),
      m_luaBrowser(ImGuiFileBrowserFlags_ConfirmOnEnter),
      m_textureBrowser(ImGuiFileBrowserFlags_ConfirmOnEnter)
{
    m_luaBrowser.SetTitle("Select Lua Script");
    m_luaBrowser.SetTypeFilters({ ".lua" });
    m_luaBrowser.SetDirectory(kScriptStockDir.string());

    m_textureBrowser.SetTitle("Select Diffuse Texture");
    m_textureBrowser.SetTypeFilters({ ".png", ".jpg", ".jpeg" });
    m_textureBrowser.SetDirectory(kTextureRootDir.string());
}

void InspectorNodeProperties::DrawWindow(bool windowState, Node* pNode)
{
    m_isOpen = windowState;
    if (!m_isOpen) return;

    float menuBarHeight = ImGui::GetFrameHeight();
    ImGui::SetNextWindowPos(ImVec2(m_screenWidth - 400, menuBarHeight), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400, m_screenHeight - menuBarHeight), ImGuiCond_Always);
    ImGui::Begin("Inspector", &m_isOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if (pNode)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 1.0f, 1.0f));
        ImGui::Text("Node Properties");
        ImGui::PopStyleColor();

        SerializedObject nodeObject;
        pNode->Serialize(nodeObject);
        std::string nodeType = nodeObject.GetType();
        if (nodeType.empty())
        {
            nodeType = "Node";
        }

        ImGui::Text("Node Type: %s", nodeType.c_str());
        ImGui::Separator();

        if (pNode != m_pSelectedNode || m_pImguiEditor->m_pRaylibEditor->IsGizmoDirty() || m_isDirty)
        {
            m_currentDatas = m_pImguiEditor->LoadInspectorData();
            m_pSelectedNode = pNode;
            m_pImguiEditor->m_pRaylibEditor->UpdateDirtyGizmo();
            m_isDirty = false;
        }

        bool wasModified = DrawDatas(m_currentDatas);
        wasModified |= DrawLuaScriptPicker(m_currentDatas);
        wasModified |= DrawTexturePicker(m_currentDatas);

        if (wasModified)
        {
            m_pImguiEditor->ApplyInspectorChanges(m_currentDatas);
        }

        ImGui::Separator();

        bool isSceneRoot = (m_pSelectedNode->GetParent() == nullptr);

        if (isSceneRoot)
        {
            ImGui::BeginDisabled();
        }

        if (ImGui::Button("Delete Node", ImVec2(-1, 0)))
        {
            if (!isSceneRoot)
            {
                EditorCommand command = {};
                command.type = EditorCommand::Type::DELETE_NODE;
                command.pNodeParam = m_pSelectedNode;
                m_pSelectedNode = nullptr;
                m_pImguiEditor->ResetSelectedNode();
                m_pImguiEditor->SetCommand(command);
            }
        }

        if (isSceneRoot)
        {
            ImGui::EndDisabled();
            ImGui::TextDisabled("Cannot delete scene root");
        }
    }
    else
    {
        ImGui::TextDisabled("No node selected");
        ImGui::Spacing();
        ImGui::TextWrapped("Select a node in the Hierarchy panel to view and edit its properties.");
    }

    ImGui::End();
}

static std::string FormatInspectorLabel(std::string const& key)
{
    if (key.empty()) return {};

    std::string label;
    size_t index = 0;

    if (key.rfind("m_", 0) == 0 && key.size() > 2)
        index = 2;

    for (; index < key.size(); ++index)
    {
        char c = key[index];
        if (c == '_')
        {
            if (!label.empty() && label.back() != ' ')
                label.push_back(' ');
            continue;
        }

        unsigned char uc = static_cast<unsigned char>(c);

        if (label.empty())
            label.push_back(static_cast<char>(std::toupper(uc)));
        else
        {
            if (std::isupper(uc) && label.back() != ' ')
                label.push_back(' ');
            label.push_back(c);
        }
    }

    return label.empty() ? key : label;

}

bool InspectorNodeProperties::DrawDatas(json& publicDataJson)
{
    bool wasModified = false;
    for (auto& [key, value] : publicDataJson.items())
    {
        if (key == "m_scriptPath" || key == "DiffuseTexturePath")
        {
            continue;
        }

        ImGui::PushID(key.c_str());
        std::string const label = FormatInspectorLabel(key);

        if (value.is_number_float())
        {
            float floatVal = value.get<float>();

            if (ImGui::DragFloat(label.c_str(), &floatVal, 0.1f))
            {
                publicDataJson[key] = floatVal;
                wasModified = true;
            }
        }
        else if (value.is_number_integer())
        {
            int intVal = value.get<int>();

            if (ImGui::DragInt(label.c_str(), &intVal))
            {
                publicDataJson[key] = intVal;
                wasModified = true;
            }
        }
        else if (value.is_boolean())
        {
            bool boolVal = value.get<bool>();

            if (ImGui::Checkbox(label.c_str(), &boolVal))
            {
                publicDataJson[key] = boolVal;
                wasModified = true;
            }
        }
        else if (value.is_string())
        {
            std::string strVal = value.get<std::string>();
            char buffer[256];
            strncpy(buffer, strVal.c_str(), sizeof(buffer));
            ImGui::InputText(label.c_str(), buffer, sizeof(buffer), 32);
            if (ImGui::IsItemDeactivatedAfterEdit())
            {
                publicDataJson[key] = std::string(buffer);
                wasModified = true;
            }
        }
        else if (value.is_object()) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            bool objOpen = ImGui::TreeNodeEx(key.c_str(), flags);

            if (objOpen)
            {
                bool dataModified = false;
                json& publicdata = publicDataJson[key]["PUBLIC_DATAS"];
                dataModified = DrawDatas(publicdata);
                ImGui::TreePop();
                if (dataModified == true) {
                    publicDataJson[key]["PUBLIC_DATAS"] = publicdata;
                    wasModified = true;
                }
            }
        }
        ImGui::PopID();
        ImGui::Spacing();

    }
    return wasModified;
}

bool InspectorNodeProperties::DrawLuaScriptPicker(json& publicDataJson)
{
    bool wasModified = false;

    ImGui::Separator();
    ImGui::Text("Script");

    std::string scriptPath = "";
    if (publicDataJson.contains("m_scriptPath"))
    {
        scriptPath = publicDataJson["m_scriptPath"].get<std::string>();
    }

    if (!scriptPath.empty())
    {
        std::string const normalized = NormalizeScriptPathForProject(scriptPath);
        if (normalized != scriptPath)
        {
            scriptPath = normalized;
            publicDataJson["m_scriptPath"] = scriptPath;
            wasModified = true;
        }
    }

    char scriptBuffer[256] = {};
    strncpy(scriptBuffer, scriptPath.c_str(), sizeof(scriptBuffer) - 1);

    bool const isSceneRoot = (m_pSelectedNode != nullptr && m_pSelectedNode->GetParent() == nullptr);

    if (isSceneRoot)
    {
        ImGui::BeginDisabled();
    }

    ImGui::InputText("Lua Script", scriptBuffer, sizeof(scriptBuffer), ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine();
    bool const browseClicked = ImGui::Button("Browse");

    bool const clearClicked = ImGui::Button("Clear Script");

    ImGui::SameLine();
    bool const createClicked = ImGui::Button("Create Script");

    if (isSceneRoot)
    {
        ImGui::EndDisabled();

        if (!scriptPath.empty())
        {
            publicDataJson["m_scriptPath"] = "";
            wasModified = true;
        }

        m_showLuaBrowser = false;
        ImGui::TextDisabled("No Script for Root");
        return wasModified;
    }

    if (browseClicked)
    {
        m_showLuaBrowser = true;
    }

    if (clearClicked && !scriptPath.empty())
    {
        publicDataJson["m_scriptPath"] = "";
        wasModified = true;
    }

    if (createClicked)
    {
        m_newLuaScriptName[0] = '\0';
        m_createLuaError.clear();
        ImGui::OpenPopup("Create Lua Script");
    }

    if (ImGui::BeginPopupModal("Create Lua Script", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::InputText("Script name", m_newLuaScriptName, sizeof(m_newLuaScriptName));

        if (!m_createLuaError.empty())
        {
            ImGui::TextColored(ImVec4(1.0f, 0.35f, 0.35f, 1.0f), "%s", m_createLuaError.c_str());
        }

        if (ImGui::Button("Create", ImVec2(120, 0)))
        {
            std::string name = m_newLuaScriptName;

            auto notSpace = [](unsigned char c) { return !std::isspace(c); };
            name.erase(name.begin(), std::find_if(name.begin(), name.end(), notSpace));
            name.erase(std::find_if(name.rbegin(), name.rend(), notSpace).base(), name.end());

            if (name.empty())
            {
                m_createLuaError = "Script name cannot be empty.";
            }
            else
            {
                if (name.size() >= 4 && name.ends_with(".lua"))
                {
                    name = name.substr(0, name.size() - 4);
                }

                std::filesystem::create_directories(kScriptStockDir);

                std::filesystem::path const scriptFilePath = kScriptStockDir / (name + ".lua");

                if (std::filesystem::exists(scriptFilePath))
                {
                    m_createLuaError = "A script with this name already exists.";
                }
                else
                {
                    std::ofstream file(scriptFilePath, std::ios::out | std::ios::trunc);
                    if (!file.is_open())
                    {
                        m_createLuaError = "Failed to create script file.";
                    }
                    else
                    {
                        std::string nodeType = "Node";
                        if (m_pSelectedNode != nullptr)
                        {
                            SerializedObject nodeObject;
                            m_pSelectedNode->Serialize(nodeObject);
                            if (!nodeObject.GetType().empty())
                            {
                                nodeType = nodeObject.GetType();
                            }
                        }
                        file << "---@type " << nodeType << "\n";
                        file << "function OnInit()\n";
                        file << "end\n\n";
                        file << "function OnUpdate(dt)\n";
                        file << "end\n\n";
                        file << "function OnDestroy()\n";
                        file << "end\n";

                        file.close();

                        publicDataJson["m_scriptPath"] = NormalizeScriptPathForProject(scriptFilePath);
                        wasModified = true;
                        m_luaBrowser.SetDirectory(kScriptStockDir.string());
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (m_showLuaBrowser)
    {
        m_luaBrowser.Open();
        m_showLuaBrowser = false;
    }

    m_luaBrowser.SetWindowSize(m_fileBrowsingSizeX, m_fileBrowsingSizeY);
    m_luaBrowser.SetWindowPos(m_screenWidth / 2 - m_fileBrowsingSizeX / 2, m_screenHeight / 2 - m_fileBrowsingSizeY / 2);
    m_luaBrowser.Display();

    if (m_luaBrowser.HasSelected())
    {
        publicDataJson["m_scriptPath"] = NormalizeScriptPathForProject(m_luaBrowser.GetSelected());
        wasModified = true;
        m_luaBrowser.ClearSelected();
        m_luaBrowser.Close();
    }

    return wasModified;
}

bool InspectorNodeProperties::DrawTexturePicker(json& publicDataJson)
{
    if (!publicDataJson.contains("DiffuseTexturePath"))
    {
        return false;
    }

    bool wasModified = false;

    ImGui::Separator();
    ImGui::Text("Texture");

    std::string texturePath = publicDataJson["DiffuseTexturePath"].get<std::string>();
    if (texturePath.empty())
    {
        texturePath = kDefaultTexturePath;
        publicDataJson["DiffuseTexturePath"] = texturePath;
        wasModified = true;
    }
    else if (texturePath != kDefaultTexturePath)
    {
        std::string const normalized = NormalizeTexturePathForProject(texturePath);
        if (normalized != texturePath)
        {
            texturePath = normalized;
            publicDataJson["DiffuseTexturePath"] = texturePath;
            wasModified = true;
        }
    }

    char textureBuffer[256] = {};
    strncpy(textureBuffer, texturePath.c_str(), sizeof(textureBuffer) - 1);

    ImGui::InputText("Diffuse", textureBuffer, sizeof(textureBuffer), ImGuiInputTextFlags_ReadOnly);

    ImGui::SameLine();
    bool const browseClicked = ImGui::Button("Browse Texture");

    ImGui::SameLine();
    bool const clearClicked = ImGui::Button("Clear Texture");

    if (browseClicked)
    {
        m_showTextureBrowser = true;
    }

    if (clearClicked && texturePath != kDefaultTexturePath)
    {
        publicDataJson["DiffuseTexturePath"] = kDefaultTexturePath;
        wasModified = true;
    }

    if (m_showTextureBrowser)
    {
        m_textureBrowser.Open();
        m_showTextureBrowser = false;
    }

    m_textureBrowser.SetWindowSize(m_fileBrowsingSizeX, m_fileBrowsingSizeY);
    m_textureBrowser.SetWindowPos(m_screenWidth / 2 - m_fileBrowsingSizeX / 2, m_screenHeight / 2 - m_fileBrowsingSizeY / 2);
    m_textureBrowser.Display();

    if (m_textureBrowser.HasSelected())
    {
        publicDataJson["DiffuseTexturePath"] = NormalizeTexturePathForProject(m_textureBrowser.GetSelected());
        wasModified = true;
        m_textureBrowser.ClearSelected();
        m_textureBrowser.Close();
    }

    return wasModified;
}