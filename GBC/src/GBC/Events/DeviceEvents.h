#pragma once

#include "Event.h"

namespace gbc
{
	class JoystickConnectEvent : public Event
	{
	public:
		JoystickConnectEvent(int jid, bool connect);
		EVENT_CLASS_TYPE(JoystickConnect)

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
		EVENT_CLASS_TYPE(MonitorConnect)

		inline bool HasConnected() const { return connect; }

		virtual std::string ToString() const override;
	private:
		bool connect;
	};
}