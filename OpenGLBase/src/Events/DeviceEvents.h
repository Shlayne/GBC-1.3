#pragma once

#include "Event.h"

namespace cbc
{
	class JoystickConnectEvent : public Event
	{
	public:
		JoystickConnectEvent(int jid, bool connect);

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

		inline bool HasConnected() const { return connect; }

		virtual std::string ToString() const override;
	private:
		bool connect;
	};
}