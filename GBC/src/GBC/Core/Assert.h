#pragma once

#include <filesystem>

#if GBC_ENABLE_ASSERTS
	#define GBC_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { GBC##type##ERROR(msg, __VA_ARGS__); GBC_DEBUGBREAK(); } }
	#define GBC_INTERNAL_ASSERT_WITH_MSG(type, check, ...) GBC_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define GBC_INTERNAL_ASSERT_NO_MSG(type, check) GBC_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", GBC_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
	#define GBC_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define GBC_INTERNAL_ASSERT_GET_MACRO(...) GBC_EXPAND_MACRO(GBC_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, GBC_INTERNAL_ASSERT_WITH_MSG, GBC_INTERNAL_ASSERT_NO_MSG))
	#define GBC_ASSERT(...) GBC_EXPAND_MACRO(GBC_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
	#define GBC_CORE_ASSERT(...) GBC_EXPAND_MACRO(GBC_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
	#define GBC_CORE_ASSERT(...)
	#define GBC_ASSERT(...)
#endif
