#include "InspectorNodePropreties.h"
#include <imgui.h>
#include <Serialization/SerializeObject.hpp>


void InspectorNodePropreties::Draw(SerializedObject& selected)
{

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 1.0f, 1.0f));
	ImGui::Text("Node Properties");
	ImGui::PopStyleColor();
	ImGui::Separator();

	// editable node name
	char nameBuffer[128];
	std::string out;

	selected.GetPublicElement("m_name", &out);
	strncpy_s(nameBuffer, out.c_str(), sizeof(nameBuffer));

	ImGui::Spacing();
	ImGui::Text("Name :");

	ImGui::SameLine();
	ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer), 32);
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		out.clear();
		out.append(nameBuffer);
		selected.AddPublicElement("m_name", &out);
		json test = selected.GetJson();
		auto ttt = test["PUBLIC_DATAS"]["m_name"].type();
		std::cout << "[EditorImGui] Node renamed to: " << nameBuffer << std::endl;
	}
	ImGui::Spacing();
	/*ImGui::Text("Parent: %s",
		selected->GetParent()
		? selected->GetParent()->GetName().c_str()
		: "None");*/

	//ImGui::Text("Children Count: %d", selected->GetChildCount());
	ImGui::Separator();
	ImGui::Spacing();
}