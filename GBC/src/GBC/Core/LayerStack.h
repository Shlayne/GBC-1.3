#pragma once

#include <vector>
#include "Layer.h"

namespace gbc
{
	class LayerStack
	{
	public:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		bool PopLayer(Layer* layer);
		bool PopOverlay(Layer* overlay);

		inline std::vector<Layer*>::iterator begin() { return layers.begin(); }
		inline std::vector<Layer*>::iterator end() { return layers.end(); }
		inline std::vector<Layer*>::reverse_iterator rbegin() { return layers.rbegin(); }
		inline std::vector<Layer*>::reverse_iterator rend() { return layers.rend(); }
		inline std::vector<Layer*>::const_iterator cbegin() const { return layers.cbegin(); }
		inline std::vector<Layer*>::const_iterator cend() const { return layers.cend(); }
		inline std::vector<Layer*>::const_reverse_iterator crbegin() const { return layers.crbegin(); }
		inline std::vector<Layer*>::const_reverse_iterator crend() const { return layers.crend(); }
	private:
		std::vector<Layer*> layers;
		size_t layerInsertIndex = 0;
	};
}
