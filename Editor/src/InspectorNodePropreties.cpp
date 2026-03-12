#include "InspectorNodePropreties.h"
#include "EditorImGui.h"
#include "EditorRaylib3D.h"

#include <Node.h>
#include <Serialization/SerializeObject.hpp>
#include <iostream>
#include <imgui.h>

void InspectorNodePropreties::DrawWindow(bool windowState, Node* pNode)
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
		ImGui::Separator();
		
		if (pNode != m_pSelectedNode || m_pImguiEditor->m_pRaylibEditor->IsGizmoDirty())
		{
			m_currentDatas = m_pImguiEditor->LoadInspectorData();
			m_pSelectedNode = pNode;
			m_pImguiEditor->m_pRaylibEditor->UpdateDirtyGizmo();
		}

		if (DrawDatas(m_currentDatas))
		{
			m_pImguiEditor->ApplyInspectorChanges(m_currentDatas);
		}

		ImGui::Separator();

		// Delete button
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

bool InspectorNodePropreties::DrawDatas(json& publicDataJson)
{
	bool wasModified = false;
	for (auto& [key, value] : publicDataJson.items())
	{
		ImGui::PushID(key.c_str());

		if (value.is_number_float())
		{
			float floatVal = value.get<float>();
			
			if (ImGui::DragFloat(key.c_str(), &floatVal, 0.1f))
			{
				// Direcly the JSON
				publicDataJson[key] = floatVal;
				wasModified = true;
			}
		}
		else if (value.is_number_integer())
		{
			int intVal = value.get<int>();
			
			if (ImGui::DragInt(key.c_str(), &intVal))
			{
				publicDataJson[key] = intVal;
				wasModified = true;
			}
		}
		else if (value.is_boolean())
		{
			bool boolVal = value.get<bool>();
			
			if (ImGui::Checkbox(key.c_str(), &boolVal))
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
			ImGui::InputText(key.c_str(), buffer, sizeof(buffer), 32);
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