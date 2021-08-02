#pragma once

#include "GBC/Core/Core.h"
#if GBC_ENABLE_IMGUI
#include <string>
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

		static void BeginTable(const std::string& id, int columnCount);
		static void NextTableColumn();
		static void PrevTableColumn();
		static void EndTable();

		template<typename... Args>
		static void Text(const std::string& format, Args&&... args)
		{
			ImGui::Text(format.c_str(), std::forward<Args>(args)...);
		}
		template<typename... Args>
		static void Text(const std::string& label, const std::string& format, Args&&... args)
		{
			Text(label);
			NextTableColumn();
			Text(format, std::forward<Args>(args)...);
		}

		static bool Float3Edit(float* values, float resetValue = defaultResetValue, float speed = defaultSpeed);
		static bool Float3Edit(const std::string& label, float* values, float resetValue = defaultResetValue, float speed = defaultSpeed);
		
		static bool FloatEdit(float* value, float speed = defaultSpeed);
		static bool FloatEdit(const std::string& label, float* value, float speed = defaultSpeed);
		
		static bool Combo(int* selectedItem, const char* const* names, int count);
		static bool Combo(const std::string& label, int* selectedItem, const char* const* names, int count);
		
		static bool Checkbox(bool* value);
		static bool Checkbox(const std::string& label, bool* value);
		
		static bool ColorEdit3(float* values);
		static bool ColorEdit3(const std::string& label, float* values);
		
		static bool ColorEdit4(float* values);
		static bool ColorEdit4(const std::string& label, float* values);
		
		static bool TextEdit(std::string* value);
		static bool TextEdit(const std::string& label, std::string* value);
	};
}
#endif
