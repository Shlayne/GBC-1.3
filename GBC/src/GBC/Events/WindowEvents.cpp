#include "gbcpch.h"
#include "WindowEvents.h"

namespace gbc
{
	WindowEvent::WindowEvent(void* nativeWindow)
		: nativeWindow(nativeWindow) {}

	WindowCloseEvent::WindowCloseEvent(void* nativeWindow)
		: WindowEvent(nativeWindow) {}

#if GBC_ENABLE_LOGGING
	std::string WindowCloseEvent::ToString() const
	{
		return "WindowCloseEvent";
	}
#endif

	WindowResizeEvent::WindowResizeEvent(void* nativeWindow, int32_t width, int32_t height)
		: WindowEvent(nativeWindow), width(width), height(height) {}

#if GBC_ENABLE_LOGGING
	std::string WindowResizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowResizeEvent: width=" << width << " height=" << height;
		return stream.str();
	}
#endif

	WindowMoveEvent::WindowMoveEvent(void* nativeWindow, int32_t x, int32_t y)
		: WindowEvent(nativeWindow), x(x), y(y) {}

#if GBC_ENABLE_LOGGING
	std::string WindowMoveEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMoveEvent: x=" << x << " y=" << y;
		return stream.str();
	}
#endif

	WindowFocusEvent::WindowFocusEvent(void* nativeWindow, bool focus)
		: WindowEvent(nativeWindow), focus(focus) {}

#if GBC_ENABLE_LOGGING
	std::string WindowFocusEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowFocusEvent: focused=" << focus;
		return stream.str();
	}
#endif

	WindowMinimizeEvent::WindowMinimizeEvent(void* nativeWindow, bool minimize)
		: WindowEvent(nativeWindow), minimize(minimize) {}

#if GBC_ENABLE_LOGGING
	std::string WindowMinimizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMinimizeEvent: minimized=" << minimize;
		return stream.str();
	}
#endif

	WindowMaximizeEvent::WindowMaximizeEvent(void* nativeWindow, bool maximize)
		: WindowEvent(nativeWindow), maximize(maximize) {}

#if GBC_ENABLE_LOGGING
	std::string WindowMaximizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMaximizeEvent: maximized=" << maximize;
		return stream.str();
	}
#endif

	WindowDropEvent::WindowDropEvent(void* nativeWindow, int32_t pathCount, const char** paths)
		: WindowEvent(nativeWindow), pathCount(pathCount), paths(paths) {}

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

	WindowFramebufferResizeEvent::WindowFramebufferResizeEvent(void* nativeWindow, int32_t width, int32_t height)
		: WindowEvent(nativeWindow), width(width), height(height) {}

#if GBC_ENABLE_LOGGING
	std::string WindowFramebufferResizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowFramebufferResizeEvent: width=" << width << " height=" << height;
		return stream.str();
	}
#endif

	WindowContentScaleEvent::WindowContentScaleEvent(void* nativeWindow, float scaleX, float scaleY)
		: WindowEvent(nativeWindow), scaleX(scaleX), scaleY(scaleY) {}

#if GBC_ENABLE_LOGGING
	std::string WindowContentScaleEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowContentScaleEvent: scaleX=" << scaleX << " scaleY=" << scaleY;
		return stream.str();
	}
#endif

	WindowRefreshEvent::WindowRefreshEvent(void* nativeWindow)
		: WindowEvent(nativeWindow) {}

#if GBC_ENABLE_LOGGING
	std::string WindowRefreshEvent::ToString() const
	{
		return "WindowContentScaleEvent";
	}
#endif
}
