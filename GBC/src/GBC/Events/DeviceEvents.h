#pragma once

#include "Event.h"

namespace gbc
{
	class JoystickConnectEvent : public Event
	{
	public:
		JoystickConnectEvent(int jid, bool connect);
		EVENT_CLASS_TYPE(EventType::JoystickConnect)
		EVENT_CLASS_CATEGORY(EventCategory_Application | EventCategory_Device)

		inline bool GetJID() const { return jid; }
		inline bool HasConnected() const { return connect; }

		virtual std::string ToString() const override;
	private:
		int jid;
		bool connect;
	};

	class MonitorConnectEvent : public Event
	{
	public:
		MonitorConnectEvent(bool connect);
		EVENT_CLASS_TYPE(EventType::MonitorConnect)
		EVENT_CLASS_CATEGORY(EventCategory_Application | EventCategory_Device)

		inline bool HasConnected() const { return connect; }

		virtual std::string ToString() const override;
	private:
		bool connect;
	};
}