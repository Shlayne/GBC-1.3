#include "WindowEvents.h"
#include <sstream>

namespace cbc
{
	WindowCloseEvent::WindowCloseEvent()
		: Event(EventType::WindowClose)
	{

	}

	std::string WindowCloseEvent::ToString() const
	{
		return "WindowCloseEvent";
	}

	WindowResizeEvent::WindowResizeEvent(int width, int height)
		: Event(EventType::WindowResize), width(width), height(height)
	{

	}

	std::string WindowResizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowResizeEvent: width=" << width << " height=" << height;
		return stream.str();
	}


	WindowMoveEvent::WindowMoveEvent(int x, int y)
		: Event(EventType::WindowMove), x(x), y(y)
	{

	}

	std::string WindowMoveEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMoveEvent: x=" << x << " y=" << y;
		return stream.str();
	}

	WindowFocusEvent::WindowFocusEvent(bool focus)
		: Event(EventType::WindowFocus), focus(focus)
	{

	}

	std::string WindowFocusEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowFocusEvent: focused=" << focus;
		return stream.str();
	}

	WindowMinimizeEvent::WindowMinimizeEvent(bool minimize)
		: Event(EventType::WindowMinimize), minimize(minimize)
	{

	}

	std::string WindowMinimizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMinimizeEvent: minimized=" << minimize;
		return stream.str();
	}

	WindowMaximizeEvent::WindowMaximizeEvent(bool maximize)
		: Event(EventType::WindowMaximize), maximize(maximize)
	{

	}

	std::string WindowMaximizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowMaximizeEvent: maximized=" << maximize;
		return stream.str();
	}

	WindowDropEvent::WindowDropEvent(int pathCount, const char** paths)
		: Event(EventType::WindowDrop), pathCount(pathCount), paths(new char*[pathCount])
	{
		for (int i = 0; i < pathCount; i++)
		{
			size_t length = strlen(paths[i]) + 1;
			this->paths[i] = new char[length];
			strcpy_s(this->paths[i], length * sizeof(char), paths[i]);
		}
	}

	WindowDropEvent::~WindowDropEvent()
	{
		for (int i = 0; i < pathCount; i++)
			delete[] paths[i];
		delete[] paths;
	}

	std::string WindowDropEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowDropEvent: pathCount=" << pathCount << " paths:";
		for (int i = 0; i < pathCount; i++)
			stream << " \"" << paths[i] << '"';
		return stream.str();
	}

	WindowFramebufferResizeEvent::WindowFramebufferResizeEvent(int width, int height)
		: Event(EventType::WindowFramebufferResize), width(width), height(height)
	{

	}

	std::string WindowFramebufferResizeEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowFramebufferResizeEvent: width=" << width << " height=" << height;
		return stream.str();
	}

	WindowContentScaleEvent::WindowContentScaleEvent(float scaleX, float scaleY)
		: Event(EventType::WindowContentScale), scaleX(scaleX), scaleY(scaleY)
	{

	}

	std::string WindowContentScaleEvent::ToString() const
	{
		std::stringstream stream;
		stream << "WindowContentScaleEvent: scaleX=" << scaleX << " scaleY=" << scaleY;
		return stream.str();
	}

	WindowRefreshEvent::WindowRefreshEvent()
		: Event(EventType::WindowRefresh)
	{

	}

	std::string WindowRefreshEvent::ToString() const
	{
		return "WindowContentScaleEvent";
	}
}
