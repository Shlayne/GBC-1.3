#include "gbcpch.h"
#include "LayerStack.h"

namespace gbc
{
	void LayerStack::PushLayer(Layer* layer)
	{
		GBC_CORE_ASSERT(std::find(layers.begin(), layers.begin() + layerInsertIndex, layer) == layers.begin() + layerInsertIndex, "Cannot add the same layer twice!");
		layers.emplace(layers.begin() + layerInsertIndex++, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		GBC_CORE_ASSERT(std::find(layers.begin() + layerInsertIndex, layers.end(), overlay) == layers.end(), "Cannot add the same overlay twice!");
		layers.emplace_back(overlay);
	}

	bool LayerStack::PopLayer(Layer* layer)
	{
		auto itEnd = layers.begin() + layerInsertIndex;
		auto it = std::find(layers.begin(), itEnd, layer);
		if (it != itEnd)
		{
			layers.erase(it);
			layerInsertIndex--;
			return true;
		}
		return false;
	}

	bool LayerStack::PopOverlay(Layer* overlay)
	{
		auto itEnd = layers.end();
		auto it = std::find(layers.begin() + layerInsertIndex, itEnd, overlay);
		if (it != itEnd)
		{
			layers.erase(it);
			return true;
		}
		return false;
	}
}
