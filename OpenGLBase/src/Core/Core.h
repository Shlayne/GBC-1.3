#pragma once

// Macros
#if CBC_DEBUG
#define CBC_ENABLE_ASSERTS 1
#define CBC_ENABLE_LOGGING 1
#define CBC_ENABLE_IMGUI 1
#define CBC_ENABLE_STATS 1
#elif CBC_RELEASE
#define CBC_ENABLE_IMGUI 1
#define CBC_ENABLE_STATS 1
#elif CBC_DIST

#endif

#if CBC_ENABLE_ASSERTS
#define CBC_ASSERT(cond, ...) if (!(cond)) { __debugbreak(); CBC_CORE_FATAL(__VA_ARGS__); }
#else
#define CBC_ASSERT(cond, ...)
#endif

#define CBC_BIND_FUNC(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

#include <memory>

namespace cbc
{
	// Ref and Scope
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{ return std::make_shared<T>(std::forward<Args>(args)...); }

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{ return std::make_unique<T>(std::forward<Args>(args)...); }

	// RendererID
	using RendererID = unsigned int;
}
