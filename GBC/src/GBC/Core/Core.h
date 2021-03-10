#pragma once

// Macros
#if GBC_CONFIG_DEBUG
#define GBC_ENABLE_ASSERTS 1
#define GBC_ENABLE_LOGGING 1
#define GBC_ENABLE_IMGUI 1
#define GBC_ENABLE_STATS 1
#elif GBC_CONFIG_RELEASE
#define GBC_ENABLE_IMGUI 1
#define GBC_ENABLE_STATS 1
#elif GBC_CONFIG_DIST

#endif

#if GBC_ENABLE_ASSERTS
#define GBC_ASSERT(cond, ...) if (!(cond)) { __debugbreak(); GBC_CORE_FATAL(__VA_ARGS__); }
#else
#define GBC_ASSERT(cond, ...)
#endif

#define GBC_BIND_FUNC(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

namespace gbc
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
