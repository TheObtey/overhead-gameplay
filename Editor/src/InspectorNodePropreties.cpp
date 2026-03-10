#include "InspectorNodePropreties.h"
#include <imgui.h>

bool InspectorNodePropreties::Draw(json& publicDataJson)
{
	bool wasModified = false;

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 1.0f, 1.0f));
	ImGui::Text("Node Properties");
	ImGui::PopStyleColor();
	ImGui::Separator();

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
			strncpy_s(buffer, strVal.c_str(), sizeof(buffer));
			
			if (ImGui::InputText(key.c_str(), buffer, sizeof(buffer)))
			{
				publicDataJson[key] = std::string(buffer);
				wasModified = true;
			}
		}

		ImGui::PopID();
	}

	ImGui::Separator();

	//ImGui::Text("Raw JSON:");
	//std::string jsonStr = publicDataJson.dump(2);
	//ImGui::TextWrapped("%s", jsonStr.c_str());

	return wasModified;
}