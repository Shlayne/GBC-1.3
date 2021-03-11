#pragma once

namespace gbc
{
	class ImGuiWrapper
	{
	public:
		ImGuiWrapper();
		~ImGuiWrapper();

		void Begin();
		void End();
	private:
		void Init();
	};
}