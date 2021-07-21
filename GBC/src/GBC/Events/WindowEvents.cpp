#include "gbcpch.h"
#include "WindowEvents.h"

namespace gbc
{
	std::string WindowCloseEvent::ToString() const
	{
		return "WindowCloseEvent";
	}

	WindowResizeEvent::WindowResizeEvent(int width, int height)
		: width(width), height(height) {}

	std::string WindowResizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowResizeEvent: width=" << width << " height=" << height;
		return stream.str();
	}


	WindowMoveEvent::WindowMoveEvent(int x, int y)
		: x(x), y(y) {}

	std::string WindowMoveEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMoveEvent: x=" << x << " y=" << y;
		return stream.str();
	}

	WindowFocusEvent::WindowFocusEvent(bool focus)
		: focus(focus) {}

	std::string WindowFocusEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowFocusEvent: focused=" << focus;
		return stream.str();
	}

	WindowMinimizeEvent::WindowMinimizeEvent(bool minimize)
		: minimize(minimize) {}

	std::string WindowMinimizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMinimizeEvent: minimized=" << minimize;
		return stream.str();
	}

	WindowMaximizeEvent::WindowMaximizeEvent(bool maximize)
		: maximize(maximize) {}

	std::string WindowMaximizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMaximizeEvent: maximized=" << maximize;
		return stream.str();
	}

	WindowDropEvent::WindowDropEvent(int pathCount, const char** paths)
		: pathCount(pathCount), paths(paths) {}

	std::string WindowDropEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowDropEvent: pathCount=" << pathCount << " paths:";
		for (int i = 0; i < pathCount; i++)
			stream << " \"" << paths[i] << '"';
		return stream.str();
	}

	WindowFramebufferResizeEvent::WindowFramebufferResizeEvent(int width, int height)
		: width(width), height(height) {}

	std::string WindowFramebufferResizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowFramebufferResizeEvent: width=" << width << " height=" << height;
		return stream.str();
	}

	WindowContentScaleEvent::WindowContentScaleEvent(float scaleX, float scaleY)
		: scaleX(scaleX), scaleY(scaleY) {}

	std::string WindowContentScaleEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowContentScaleEvent: scaleX=" << scaleX << " scaleY=" << scaleY;
		return stream.str();
	}

	std::string WindowRefreshEvent::ToString() const
	{
		return "WindowContentScaleEvent";
	}
}
