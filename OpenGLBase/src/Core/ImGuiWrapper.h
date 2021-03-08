#pragma once

#include "Window.h"

class ImGuiWrapper
{
public:
	ImGuiWrapper(const Window& window);
	~ImGuiWrapper();

	void Begin();
	void End();
private:
	void Init();

	const Window& window;
};
