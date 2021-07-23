#include "gbcpch.h"
#include "WindowEvents.h"

namespace gbc
{
#if GBC_ENABLE_LOGGING
	std::string WindowCloseEvent::ToString() const
	{
		return "WindowCloseEvent";
	}
#endif

	WindowResizeEvent::WindowResizeEvent(int width, int height)
		: width(width), height(height) {}

#if GBC_ENABLE_LOGGING
	std::string WindowResizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowResizeEvent: width=" << width << " height=" << height;
		return stream.str();
	}
#endif

	WindowMoveEvent::WindowMoveEvent(int x, int y)
		: x(x), y(y) {}

#if GBC_ENABLE_LOGGING
	std::string WindowMoveEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMoveEvent: x=" << x << " y=" << y;
		return stream.str();
	}
#endif

	WindowFocusEvent::WindowFocusEvent(bool focus)
		: focus(focus) {}

#if GBC_ENABLE_LOGGING
	std::string WindowFocusEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowFocusEvent: focused=" << focus;
		return stream.str();
	}
#endif

	WindowMinimizeEvent::WindowMinimizeEvent(bool minimize)
		: minimize(minimize) {}

#if GBC_ENABLE_LOGGING
	std::string WindowMinimizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMinimizeEvent: minimized=" << minimize;
		return stream.str();
	}
#endif

	WindowMaximizeEvent::WindowMaximizeEvent(bool maximize)
		: maximize(maximize) {}

#if GBC_ENABLE_LOGGING
	std::string WindowMaximizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMaximizeEvent: maximized=" << maximize;
		return stream.str();
	}
#endif

	WindowDropEvent::WindowDropEvent(int pathCount, const char** paths)
		: pathCount(pathCount), paths(paths) {}

#if GBC_ENABLE_LOGGING
	std::string WindowDropEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowDropEvent: pathCount=" << pathCount << " paths:";
		for (int i = 0; i < pathCount; i++)
			stream << " \"" << paths[i] << '"';
		return stream.str();
	}
#endif

	WindowFramebufferResizeEvent::WindowFramebufferResizeEvent(int width, int height)
		: width(width), height(height) {}

#if GBC_ENABLE_LOGGING
	std::string WindowFramebufferResizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowFramebufferResizeEvent: width=" << width << " height=" << height;
		return stream.str();
	}
#endif

	WindowContentScaleEvent::WindowContentScaleEvent(float scaleX, float scaleY)
		: scaleX(scaleX), scaleY(scaleY) {}

#if GBC_ENABLE_LOGGING
	std::string WindowContentScaleEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowContentScaleEvent: scaleX=" << scaleX << " scaleY=" << scaleY;
		return stream.str();
	}
#endif

#if GBC_ENABLE_LOGGING
	std::string WindowRefreshEvent::ToString() const
	{
		return "WindowContentScaleEvent";
	}
#endif
}
