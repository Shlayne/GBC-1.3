#pragma once

class ImGuiWrapper
{
public:
	ImGuiWrapper(void* window);
	~ImGuiWrapper();

	void Begin();
	void End();
private:
	void Init();
};
