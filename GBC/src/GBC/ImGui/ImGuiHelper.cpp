#include "gbcpch.h"
#if GBC_ENABLE_IMGUI
#include "ImGuiHelper.h"
#include <imgui/imgui_internal.h>

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

		io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);
		data.boldFont = io.Fonts->AddFontFromFileTTF("Resources/Fonts/OpenSans/OpenSans-Bold.ttf", 18.0f);

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		style.WindowRounding = 0.0f;
		style.FramePadding = { 4.0f, 4.0f };
		style.WindowPadding = { 4.0f, 4.0f };

		SetSceneEditColors();
	}

	void ImGuiHelper::SetScenePlayColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		SetSceneEditColors();

		// TODO: this could most definitely use some actual hardcoded colors and not just a tint
		for (int32_t i = 0; i < ImGuiCol_COUNT; i++)
			colors[i] = { colors[i].x * 0.8f, colors[i].y * 0.8f, colors[i].z * 1.0f, colors[i].w };
	}

	void ImGuiHelper::SetSceneEditColors()
	{
		auto& colors = ImGui::GetStyle().Colors;

		// Start with dark theme
		ImGui::StyleColorsDark();

		// Window
		colors[ImGuiCol_WindowBg]           = { 0.100f, 0.105f, 0.110f, 1.000f };

		// Frame Background
		colors[ImGuiCol_FrameBg]            = { 0.165f, 0.170f, 0.175f, 1.000f };
		colors[ImGuiCol_FrameBgHovered]     = { 0.200f, 0.205f, 0.210f, 1.000f };
		colors[ImGuiCol_FrameBgActive]      = { 0.150f, 0.155f, 0.160f, 1.000f };

		// Title Background
		colors[ImGuiCol_TitleBg]            = { 0.150f, 0.155f, 0.160f, 1.000f };
		colors[ImGuiCol_TitleBgActive]      = { 0.150f, 0.155f, 0.160f, 1.000f };
		colors[ImGuiCol_TitleBgCollapsed]   = { 0.150f, 0.155f, 0.160f, 1.000f };
		
		// Checkbox
		colors[ImGuiCol_CheckMark]          = { 0.400f, 0.405f, 0.410f, 1.000f };

		// Slider
		colors[ImGuiCol_SliderGrab]         = { 0.338f, 0.338f, 0.338f, 1.000f };
		colors[ImGuiCol_SliderGrabActive]   = { 0.338f, 0.338f, 0.338f, 1.000f };

		// Buttons
		colors[ImGuiCol_Button]             = { 0.200f, 0.205f, 0.210f, 1.000f };
		colors[ImGuiCol_ButtonHovered]      = { 0.300f, 0.305f, 0.310f, 1.000f };
		colors[ImGuiCol_ButtonActive]       = { 0.150f, 0.155f, 0.160f, 1.000f };

		// Headers
		colors[ImGuiCol_Header]             = { 0.200f, 0.205f, 0.210f, 1.000f };
		colors[ImGuiCol_HeaderHovered]      = { 0.300f, 0.305f, 0.310f, 1.000f };
		colors[ImGuiCol_HeaderActive]       = { 0.150f, 0.155f, 0.160f, 1.000f };

		// Tabs
		colors[ImGuiCol_Tab]                = { 0.150f, 0.155f, 0.160f, 1.000f };
		colors[ImGuiCol_TabHovered]         = { 0.380f, 0.385f, 0.390f, 1.000f };
		colors[ImGuiCol_TabActive]          = { 0.280f, 0.285f, 0.290f, 1.000f };
		colors[ImGuiCol_TabUnfocused]       = { 0.150f, 0.155f, 0.160f, 1.000f };
		colors[ImGuiCol_TabUnfocusedActive] = { 0.200f, 0.205f, 0.210f, 1.000f };
	}

	bool ImGuiHelper::BeginTable(const char* id, int32_t columnCount, ImGuiTableFlags tableFlags)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x / 2.0f, style.ItemSpacing.y));

		ImGui::PushID(id);
		if (!ImGui::BeginTable(id, columnCount, tableFlags))
		{
			ImGui::PopID();
			ImGui::PopStyleVar();
			return false;
		}

		ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthAuto;
		ImGui::TableSetupColumn(nullptr, columnFlags);
		ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_None);
		for (int32_t i = 2; i < columnCount; i++)
			ImGui::TableSetupColumn(nullptr, columnFlags);

		NextTableColumn();
		return true;
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

	bool ImGuiHelper::InputText(char* buffer, size_t size, ImGuiInputTextFlags flags)
	{
		ImGui::SetKeyboardFocusHere();
		return ImGui::InputText("", buffer, size, flags) || (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left));
	}

	bool ImGuiHelper::FloatEdit3(float* values, float resetValue, float speed)
	{
		bool changed = false;
		ImGui::PushItemWidth(-FLT_MIN);

		changed |= ImGui::DragFloat3("", values, speed);

		ImGui::PopItemWidth();
		return changed;
	}

	bool ImGuiHelper::FloatEdit3(const char* label, float* values, float resetValue, float speed)
	{
		bool changed = false;
		ImGui::PushID(label);

		NextTableColumn();
		changed |= FloatEdit3(values, resetValue, speed);
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

	bool ImGuiHelper::FloatEdit(const char* label, float* value, float speed)
	{
		bool changed = false;
		ImGui::PushID(label);

		NextTableColumn();
		changed |= FloatEdit(value, speed);
		PrevTableColumn();
		Text(label);
		NextTableColumn();

		ImGui::PopID();
		return changed;
	}

	bool ImGuiHelper::SliderFloat(float* value, float minValue, float maxValue)
	{
		bool changed = false;
		ImGui::PushItemWidth(-FLT_MIN);

		changed |= ImGui::SliderFloat("", value, minValue, maxValue);

		ImGui::PopItemWidth();
		return changed;
	}

	bool ImGuiHelper::SliderFloat(const char* label, float* value, float minValue, float maxValue)
	{
		bool changed = false;
		ImGui::PushID(label);

		NextTableColumn();
		changed |= SliderFloat(value, minValue, maxValue);
		PrevTableColumn();
		Text(label);
		NextTableColumn();

		ImGui::PopID();
		return changed;
	}

	bool ImGuiHelper::Combo(int32_t* selectedItem, const char* const* names, int32_t count)
	{
		bool changed = false;
		ImGui::PushItemWidth(-FLT_MIN);

		if (ImGui::BeginCombo("", names[*selectedItem]))
		{
			for (int32_t i = 0; i < count; i++)
			{
				bool selected = *selectedItem == i;
				if (ImGui::Selectable(names[i], &selected) && *selectedItem != i)
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

	bool ImGuiHelper::Combo(const char* label, int32_t* selectedItem, const char* const* names, int32_t count)
	{
		bool changed = false;
		ImGui::PushID(label);

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

	bool ImGuiHelper::Checkbox(const char* label, bool* value)
	{
		bool changed = false;
		ImGui::PushID(label);

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

	bool ImGuiHelper::ColorEdit3(const char* label, float* values)
	{
		bool changed = false;
		ImGui::PushID(label);

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

	bool ImGuiHelper::ColorEdit4(const char* label, float* values)
	{
		bool changed = false;
		ImGui::PushID(label);

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
		char buffer[bufferSize]{ '\0' };
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

	bool ImGuiHelper::TextEdit(const char* label, std::string* value)
	{
		bool changed = false;
		ImGui::PushID(label);

		NextTableColumn();
		changed |= TextEdit(value);
		PrevTableColumn();
		Text(label);
		NextTableColumn();

		ImGui::PopID();
		return changed;
	}

	bool ImGuiHelper::ButtonDragDropTarget(const char* buttonText, const char* dragDropType, const std::function<void(const ImGuiPayload*)>& dragDropFunc)
	{
		bool changed = false;

		ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.0f, 0.5f });
		changed |= ImGui::Button(buttonText, { ImGui::GetContentRegionAvail().x, 0.0f });
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dragDropType))
				dragDropFunc(payload);
			ImGui::EndDragDropTarget();
		}
		ImGui::PopStyleVar();

		return changed;
	}

	bool ImGuiHelper::ButtonDragDropTarget(const char* label, const char* buttonText, const char* dragDropType, const std::function<void(const ImGuiPayload*)>& dragDropFunc)
	{
		bool changed = false;
		ImGui::PushID(label);

		NextTableColumn();
		changed |= ButtonDragDropTarget(buttonText, dragDropType, dragDropFunc);
		PrevTableColumn();
		Text(label);
		NextTableColumn();

		ImGui::PopID();
		return changed;
	}
}
#endif
