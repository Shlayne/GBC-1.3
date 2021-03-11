#include "gbcpch.h"
#include "Profiler.h"

namespace gbc
{
	InstrumentationSession::InstrumentationSession(const std::string& name)
		: name(name) {}

	void Profiler::BeginSession(const std::string& name, const std::string& filepath)
	{
		std::lock_guard lock(mutex);

		if (currentSession)
			InternalEndSession();
		outputStream.open(filepath);

		if (outputStream.is_open())
		{
			currentSession = new InstrumentationSession(name);
			WriteHeader();
		}
	}

	void Profiler::EndSession()
	{
		std::lock_guard lock(mutex);
		InternalEndSession();
	}

	void Profiler::WriteProfile(const ProfileResult& result)
	{
		std::lock_guard lock(mutex);
		std::stringstream json;

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << result.elapsedTime.count() << ',';
		json << "\"name\":\"" << result.name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.threadID << ',';
		json << "\"ts\":" << result.start.count();
		json << '}';

		if (currentSession)
		{
			outputStream << json.str();
			outputStream.flush();
		}
	}

	Profiler& Profiler::Get()
	{
		static Profiler instance;
		return instance;
	}

	Profiler::Profiler()
		: currentSession(nullptr)
	{

	}

	Profiler::~Profiler()
	{
		EndSession();
	}

	void Profiler::WriteHeader()
	{
		outputStream << "{\"otherData\":{},\"traceEvents\":[{}";
		outputStream.flush();
	}

	void Profiler::WriteFooter()
	{
		outputStream << "]}";
		outputStream.flush();
	}

	void Profiler::InternalEndSession()
	{
		if (currentSession)
		{
			WriteFooter();
			outputStream.close();
			delete currentSession;
			currentSession = nullptr;
		}
	}

	ProfilerTimer::ProfilerTimer(const char* name)
		: name(name), stopped(false)
	{
		startTimepoint = std::chrono::steady_clock::now();
	}

	ProfilerTimer::~ProfilerTimer()
	{
		if (!stopped)
			Stop();
	}

	void ProfilerTimer::Stop()
	{
		auto endTimepoint = std::chrono::steady_clock::now();
		FloatingPointMicroseconds highResStart = startTimepoint.time_since_epoch();
		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch();

		Profiler::Get().WriteProfile({name, highResStart, elapsedTime, std::this_thread::get_id()});

		stopped = true;
	}
}
