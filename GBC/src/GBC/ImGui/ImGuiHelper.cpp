#include "gbcpch.h"
#include "ImGuiHelper.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace gbc
{
	struct ImGuiHelperData
	{
		ImFont* boldFont = nullptr;
	};
	static ImGuiHelperData data;

	void ImGuiHelper::Init()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);
		data.boldFont = io.Fonts->AddFontFromFileTTF("resources/fonts/OpenSans/OpenSans-Bold.ttf", 18.0f);

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard
			| ImGuiConfigFlags_DockingEnable
			| ImGuiConfigFlags_ViewportsEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		style.WindowRounding = 0.0f;
		style.FramePadding = {4.0f, 4.0f};
		style.WindowPadding = {4.0f, 4.0f};

		SetDarkthemeColors();
	}

	void ImGuiHelper::Shutdown()
	{

	}

	void ImGuiHelper::SetDarkthemeColors()
	{
		// Colors
		ImGui::StyleColorsDark();

		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = {0.1f, 0.105f, 0.11f, 1.0f};

		// Headers
		colors[ImGuiCol_Header] = {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_HeaderHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_HeaderActive] = {0.15f, 0.155f, 0.16f, 1.0f};

		// Buttons
		colors[ImGuiCol_Button] = {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_ButtonHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_ButtonActive] = {0.15f, 0.155f, 0.16f, 1.0f};

		// Frame Background
		colors[ImGuiCol_FrameBg] = {0.2f, 0.205f, 0.21f, 1.0f};
		colors[ImGuiCol_FrameBgHovered] = {0.3f, 0.305f, 0.31f, 1.0f};
		colors[ImGuiCol_FrameBgActive] = {0.15f, 0.155f, 0.16f, 1.0f};

		// Tabs
		colors[ImGuiCol_Tab] = {0.15f, 0.155f, 0.16f, 1.0f};
		colors[ImGuiCol_TabHovered] = {0.38f, 0.385f, 0.39f, 1.0f};
		colors[ImGuiCol_TabActive] = {0.28f, 0.285f, 0.29f, 1.0f};
		colors[ImGuiCol_TabUnfocused] = {0.15f, 0.155f, 0.16f, 1.0f};
		colors[ImGuiCol_TabUnfocusedActive] = {0.2f, 0.205f, 0.21f, 1.0f};

		// Title Background
		colors[ImGuiCol_TitleBg] = {0.15f, 0.155f, 0.16f, 1.0f};
		colors[ImGuiCol_TitleBgActive] = {0.15f, 0.155f, 0.16f, 1.0f};
		colors[ImGuiCol_TitleBgCollapsed] = {0.15f, 0.155f, 0.16f, 1.0f};
	}

	void ImGuiHelper::BeginColumns(const std::string& label, float percentWidth)
	{
		GBC_CORE_ASSERT(percentWidth >= 0.0f && percentWidth <= 1.0f, "Percent width out of bounds!");

		ImGui::PushID(label.c_str());
		float availableWidth = ImGui::GetContentRegionAvail().x;
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, availableWidth * (1.0f - percentWidth));
		ImGui::SetColumnWidth(1, availableWidth * percentWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, ImGui::GetStyle().FramePadding.y));
	}

	void ImGuiHelper::EndColumns()
	{
		ImGui::PopStyleVar();

		ImGui::Columns();
		ImGui::PopID();
	}

	bool ImGuiHelper::Float3Edit(const std::string& label, float* values, float resetValue, float speed, float percentWidth)
	{
		bool changed = false;
		BeginColumns(label, percentWidth);

		ImGuiIO& io = ImGui::GetIO();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = {lineHeight, lineHeight};

		ImGui::PushStyleColor(ImGuiCol_Button, {0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.9f, 0.2f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushFont(data.boldFont);
		if (ImGui::Button("X", buttonSize))
		{
			*values = resetValue;
			changed = true;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		changed |= ImGui::DragFloat("##X", values, speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, {0.2f, 0.7f, 0.3f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.3f, 0.8f, 0.3f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.2f, 0.7f, 0.3f, 1.0f});
		ImGui::PushFont(data.boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values[1] = resetValue;
			changed = true;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		changed |= ImGui::DragFloat("##Y", &values[1], speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, {0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.2f, 0.35f, 0.9f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushFont(data.boldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			values[2] = resetValue;
			changed = true;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		changed |= ImGui::DragFloat("##Z", &values[2], speed);
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		EndColumns();
		return changed;
	}

	bool ImGuiHelper::FloatEdit(const std::string& label, float* value, float speed, float percentWidth)
	{
		bool changed = false;
		BeginColumns(label, percentWidth);
		
		ImGui::PushItemWidth(-1);
		changed |= ImGui::DragFloat("", value, speed);
		ImGui::PopItemWidth();
		
		EndColumns();
		return changed;
	}
	
	bool ImGuiHelper::Combo(const std::string& label, int* selectedItem, const char* const* names, int count, float percentWidth)
	{
		bool changed = false;
		BeginColumns(label, percentWidth);

		ImGui::PushItemWidth(-1);
		if (ImGui::BeginCombo("", names[*selectedItem]))
		{
			for (int i = 0; i < count; i++)
			{
				bool selected = *selectedItem == i;
				if (ImGui::Selectable(names[i], &selected))
				{
					*selectedItem = i;
					changed = true;
				}
				if (selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		EndColumns();
		return changed;
	}

	bool ImGuiHelper::Checkbox(const std::string& label, bool* value, float percentWidth)
	{
		bool changed = false;
		BeginColumns(label, percentWidth);

		changed |= ImGui::Checkbox("", value);

		EndColumns();
		return changed;
	}

	bool ImGuiHelper::ColorEdit3(const std::string& label, float* values, float percentWidth)
	{
		bool changed = false;
		BeginColumns(label, percentWidth);

		changed |= ImGui::ColorEdit3("", values);

		EndColumns();
		return changed;
	}

	bool ImGuiHelper::ColorEdit4(const std::string& label, float* values, float percentWidth)
	{
		bool changed = false;
		BeginColumns(label, percentWidth);

		changed |= ImGui::ColorEdit4("", values);

		EndColumns();
		return changed;
	}
}
