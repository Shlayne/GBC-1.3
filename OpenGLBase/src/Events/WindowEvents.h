#pragma once

#include "Event.h"

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent();

	virtual std::string ToString() const override;
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(int width, int height);

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

	virtual std::string ToString() const override;
private:
	int width;
	int height;
};

class WindowMoveEvent : public Event
{
public:
	WindowMoveEvent(int x, int y);

	inline int GetX() const { return x; }
	inline int GetY() const { return y; }

	virtual std::string ToString() const override;
private:
	int x;
	int y;
};

class WindowFocusEvent : public Event
{
public:
	WindowFocusEvent(bool focus);

	inline bool IsFocused() const { return focus; }

	virtual std::string ToString() const override;
private:
	bool focus;
};

class WindowMinimizeEvent : public Event
{
public:
	WindowMinimizeEvent(bool minimize);

	inline bool IsMinimized() const { return minimize; }

	virtual std::string ToString() const override;
private:
	bool minimize;
};

class WindowMaximizeEvent : public Event
{
public:
	WindowMaximizeEvent(bool maximize);

	inline bool IsMaximized() const { return maximize; }

	virtual std::string ToString() const override;
private:
	bool maximize;
};

class WindowDropEvent : public Event
{
public:
	WindowDropEvent(int pathCount, const char** paths);
	~WindowDropEvent();

	inline int GetPathCount() const { return pathCount; }
	inline char** GetPaths() const { return paths; }

	virtual std::string ToString() const override;
private:
	int pathCount;
	char** paths;
};

class WindowFramebufferResizeEvent : public Event
{
public:
	WindowFramebufferResizeEvent(int width, int height);

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

	virtual std::string ToString() const override;
private:
	int width;
	int height;
};

class WindowContentScaleEvent : public Event
{
public:
	WindowContentScaleEvent(float scaleX, float scaleY);

	inline float GetScaleX() const { return scaleX; }
	inline float GetScaleY() const { return scaleY; }

	virtual std::string ToString() const override;
private:
	float scaleX;
	float scaleY;
};

class WindowRefreshEvent : public Event
{
public:
	WindowRefreshEvent();

	virtual std::string ToString() const override;
};
