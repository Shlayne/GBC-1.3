#pragma once

#include "GBC/Core/Core.h"
#if GBC_ENABLE_IMGUI
#include <string>
#include <functional>
#include "imgui/imgui.h"

namespace gbc
{
	class ImGuiHelper
	{
	private:
		static void Init();
		friend class ImGuiWrapper;

		static constexpr float defaultResetValue = 0.0f;
		static constexpr float defaultSpeed = 0.1f;
	public:
		static void SetDarkthemeColors();

		static bool BeginTable(const char* id, int columnCount, ImGuiTableFlags tableFlags = ImGuiTableFlags_BordersInnerV);
		static void NextTableColumn();
		static void PrevTableColumn();
		static void EndTable();

		template<typename... Args>
		static void Text(const char* format, Args&&... args)
		{
			ImGui::Text(format, std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Text(const char* label, const char* format, Args&&... args)
		{
			Text(label);
			NextTableColumn();
			ImGui::Text(format, std::forward<Args>(args)...);
		}

		static bool InputText(char* buffer, size_t size, ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		
		template<size_t Size>
		inline static bool InputText(char(&buffer)[Size], ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)
		{ return InputText(buffer, Size, flags); }

		static bool FloatEdit3(float* values, float resetValue = defaultResetValue, float speed = defaultSpeed);
		static bool FloatEdit3(const char* label, float* values, float resetValue = defaultResetValue, float speed = defaultSpeed);
		
		static bool FloatEdit(float* value, float speed = defaultSpeed);
		static bool FloatEdit(const char* label, float* value, float speed = defaultSpeed);

		static bool SliderFloat(float* value, float minValue = 0.0f, float maxValue = 1.0f);
		static bool SliderFloat(const char* label, float* value, float minValue = 0.0f, float maxValue = 1.0f);
		
		static bool Combo(int* selectedItem, const char* const* names, int count);
		static bool Combo(const char* label, int* selectedItem, const char* const* names, int count);
		
		static bool Checkbox(bool* value);
		static bool Checkbox(const char* label, bool* value);
		
		static bool ColorEdit3(float* values);
		static bool ColorEdit3(const char* label, float* values);
		
		static bool ColorEdit4(float* values);
		static bool ColorEdit4(const char* label, float* values);
		
		static bool TextEdit(std::string* value);
		static bool TextEdit(const char* label, std::string* value);

		static bool ButtonDragDropTarget(const char* buttonText, const char* dragDropType, const std::function<void(const ImGuiPayload*)>& dragDropFunc);
		static bool ButtonDragDropTarget(const char* label, const char* buttonText, const char* dragDropType, const std::function<void(const ImGuiPayload*)>& dragDropFunc);

		template<typename... Args>
		static bool ConfirmAction(const char* name, bool* action, const char* messageFormat, Args&&... args)
		{
			bool closed = false;

			ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, {0.5f, 0.5f});
			ImGui::OpenPopup(name);

			if (ImGui::BeginPopupModal(name, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
			{
				ImGui::Text(messageFormat, std::forward<Args>(args)...);

				float availableWidth = ImGui::GetContentRegionAvail().x;
				float padding = ImGui::GetStyle().FramePadding.x;
				auto width = availableWidth * 0.5f - padding;
				ImVec2 buttonSize = {width, 0.0f};

				if (ImGui::Button("Yes", buttonSize))
				{
					*action = true;
					closed = true;
				}

				ImGui::SameLine();

				if (ImGui::Button("No", buttonSize))
				{
					*action = false;
					closed = true;
				}

				ImGui::EndPopup();
			}

			return closed;
		}
	};
}
#endif
