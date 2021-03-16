#pragma once

#include <string>
#include "glm/glm.hpp"

namespace gbc
{
	class ImGuiHelper
	{
	private:
		static void Init();
		static void Shutdown();
		friend class ImGuiLayer;

		static constexpr float defaultPercentWidth = 0.7f;
	public:
		static void SetDarkthemeColors();

		static void BeginColumns(const std::string& label, float percentWidth = defaultPercentWidth);
		static void EndColumns();

		static bool Float3Edit(const std::string& label, float* values, float resetValue = 0.0f, float speed = 0.1f, float percentWidth = defaultPercentWidth);
		static bool FloatEdit(const std::string& label, float* value, float speed = 0.1f, float percentWidth = defaultPercentWidth);
		static bool Combo(const std::string& label, int* selectedItem, const char* const* names, int count, float percentWidth = defaultPercentWidth);
		static bool Checkbox(const std::string& label, bool* value, float percentWidth = defaultPercentWidth);
		static bool ColorEdit3(const std::string& label, float* values, float percentWidth = defaultPercentWidth);
		static bool ColorEdit4(const std::string& label, float* values, float percentWidth = defaultPercentWidth);
	};
}
