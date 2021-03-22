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

	void ImGuiHelper::BeginTable(const std::string& id, int columnCount)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x / 2.0f, style.ItemSpacing.y));

		ImGui::PushID(id.c_str());
		ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerV;
		ImGui::BeginTable(id.c_str(), columnCount, tableFlags);

		ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthAuto;
		ImGui::TableSetupColumn(nullptr, columnFlags);
		ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_None);
		for (int i = 2; i < columnCount; i++)
			ImGui::TableSetupColumn(nullptr, columnFlags);

		NextTableColumn();
	}

	void ImGuiHelper::NextTableColumn()
	{
		ImGui::TableNextColumn();
	}

	void ImGuiHelper::PrevTableColumn()
	{
		ImGui::TableSetColumnIndex(ImGui::TableGetColumnIndex() - 1);
	}

	void ImGuiHelper::EndTable()
	{
		ImGui::EndTable();
		ImGui::PopID();

		ImGui::PopStyleVar();
	}

	bool ImGuiHelper::Float3Edit(float* values, float resetValue, float speed)
	{
		bool changed = false;
		ImGui::PushItemWidth(-FLT_MIN);

		changed |= ImGui::DragFloat3("", values, speed);

		ImGui::PopItemWidth();
		return changed;
	}

	bool ImGuiHelper::Float3Edit(const std::string& label, float* values, float resetValue, float speed)
	{
		bool changed = false;
		ImGui::PushID(label.c_str());

		NextTableColumn();
		changed |= Float3Edit(values, resetValue, speed);
		PrevTableColumn();
		Text(label);
		NextTableColumn();

		ImGui::PopID();
		return changed;
	}

	bool ImGuiHelper::FloatEdit(float* value, float speed)
	{
		bool changed = false;
		ImGui::PushItemWidth(-FLT_MIN);

		changed |= ImGui::DragFloat("", value, speed);

		ImGui::PopItemWidth();
		return changed;
	}

	bool ImGuiHelper::FloatEdit(const std::string& label, float* value, float speed)
	{
		bool changed = false;
		ImGui::PushID(label.c_str());

		NextTableColumn();
		changed |= FloatEdit(value, speed);
		PrevTableColumn();
		Text(label);
		NextTableColumn();

		ImGui::PopID();
		return changed;
	}

	bool ImGuiHelper::Combo(int* selectedItem, const char* const* names, int count)
	{
		bool changed = false;
		ImGui::PushItemWidth(-FLT_MIN);

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
		return changed;
	}

	bool ImGuiHelper::Combo(const std::string& label, int* selectedItem, const char* const* names, int count)
	{
		bool changed = false;
		ImGui::PushID(label.c_str());

		NextTableColumn();
		changed |= Combo(selectedItem, names, count);
		PrevTableColumn();
		Text(label);
		NextTableColumn();

		ImGui::PopID();
		return changed;
	}

	bool ImGuiHelper::Checkbox(bool* value)
	{
		bool changed = false;
		ImGui::PushItemWidth(-FLT_MIN);

		changed |= ImGui::Checkbox("", value);

		ImGui::PopItemWidth();
		return changed;
	}

	bool ImGuiHelper::Checkbox(const std::string& label, bool* value)
	{
		bool changed = false;
		ImGui::PushID(label.c_str());

		NextTableColumn();
		changed |= Checkbox(value);
		PrevTableColumn();
		Text(label);
		NextTableColumn();

		ImGui::PopID();
		return changed;
	}

	bool ImGuiHelper::ColorEdit3(float* values)
	{
		bool changed = false;
		ImGui::PushItemWidth(-FLT_MIN);

		changed |= ImGui::ColorEdit3("", values);

		ImGui::PopItemWidth();
		return changed;
	}

	bool ImGuiHelper::ColorEdit3(const std::string& label, float* values)
	{
		bool changed = false;
		ImGui::PushID(label.c_str());

		NextTableColumn();
		changed |= ColorEdit3(values);
		PrevTableColumn();
		Text(label);
		NextTableColumn();

		ImGui::PopID();
		return changed;
	}

	bool ImGuiHelper::ColorEdit4(float* values)
	{
		bool changed = false;
		ImGui::PushItemWidth(-FLT_MIN);

		changed |= ImGui::ColorEdit4("", values);

		ImGui::PopItemWidth();
		return changed;
	}

	bool ImGuiHelper::ColorEdit4(const std::string& label, float* values)
	{
		bool changed = false;
		ImGui::PushID(label.c_str());

		NextTableColumn();
		changed |= ColorEdit4(values);
		PrevTableColumn();
		Text(label);
		NextTableColumn();

		ImGui::PopID();
		return changed;
	}

	bool ImGuiHelper::TextEdit(std::string* value)
	{
		bool changed = false;
		ImGui::PushItemWidth(-FLT_MIN);

		static constexpr size_t bufferSize = 256;
		char buffer[bufferSize]{0};
		GBC_CORE_ASSERT(value->size() < bufferSize);
		strcpy_s(buffer, sizeof(buffer), value->c_str());

		if (ImGui::InputText("", buffer, sizeof(buffer)))
		{
			*value = buffer;
			changed = true;
		}

		ImGui::PopItemWidth();
		return changed;
	}

	bool ImGuiHelper::TextEdit(const std::string& label, std::string* value)
	{
		bool changed = false;
		ImGui::PushID(label.c_str());

		NextTableColumn();
		changed |= TextEdit(value);
		PrevTableColumn();
		Text(label);
		NextTableColumn();

		ImGui::PopID();
		return changed;
	}
}
