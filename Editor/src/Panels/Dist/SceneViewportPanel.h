#pragma once

#include "Panels/Panel.h"

namespace ImGuizmo
{
	enum class OPERATION;
}

namespace gbc
{
	class SceneViewportPanel : public Panel
	{
	public:
		SceneViewportPanel(const std::string& name, EditorLayer* editorLayer);

		virtual void OnImGuiRender() override;
	};
}
