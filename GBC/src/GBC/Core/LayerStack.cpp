#include "gbcpch.h"
#include "LayerStack.h"

namespace gbc
{
	void LayerStack::PushLayer(Layer* layer)
	{
		GBC_CORE_ASSERT(std::find(layers.begin(), layers.begin() + layerInsertIndex, layer) == layers.begin() + layerInsertIndex, "Cannot add the same layer twice!");
		layers.emplace(layers.begin() + layerInsertIndex, layer);
		layerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		GBC_CORE_ASSERT(std::find(layers.begin() + layerInsertIndex, layers.end(), overlay) == layers.end(), "Cannot add the same overlay twice!");
		layers.emplace_back(overlay);
	}

	Layer* LayerStack::PopLayer()
	{
		return layerInsertIndex ? *(layers.begin() + --layerInsertIndex) : nullptr;
	}

	Layer* LayerStack::PopOverlay()
	{
		auto it = layers.begin() + layerInsertIndex;
		return it != layers.end() ? *it : nullptr;
	}
}
