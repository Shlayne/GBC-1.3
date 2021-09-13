#pragma once

#include <vector>
#include "GBC/Core/Layer.h"

namespace gbc
{
	class LayerStack
	{
	public:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		Layer* PopLayer();
		Layer* PopOverlay();

		inline auto begin() noexcept { return layers.begin(); }
		inline auto end() noexcept { return layers.end(); }
		inline auto rbegin() noexcept { return layers.rbegin(); }
		inline auto rend() noexcept { return layers.rend(); }
		inline auto cbegin() const noexcept { return layers.cbegin(); }
		inline auto cend() const noexcept { return layers.cend(); }
		inline auto crbegin() const noexcept { return layers.crbegin(); }
		inline auto crend() const noexcept { return layers.crend(); }
	private:
		std::vector<Layer*> layers;
		size_t layerInsertIndex = 0;
	};
}
