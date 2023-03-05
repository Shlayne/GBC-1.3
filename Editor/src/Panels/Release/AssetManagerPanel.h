#pragma once

#include "Panels/Panel.h"
#include "GBC/Core/AssetManager.h"

namespace gbc
{
	class AssetManagerPanel : public Panel
	{
	public:
		AssetManagerPanel(const std::string& name, EditorLayer* editorLayer);

		virtual void OnImGuiRender() override;
	public:
		std::string AssetTypeToString(Asset::Type assetType);
	};
}
