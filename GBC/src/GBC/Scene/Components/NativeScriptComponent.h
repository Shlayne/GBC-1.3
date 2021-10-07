#pragma once

namespace gbc
{
	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity*(*createFunc)();
		void(*destroyFunc)(NativeScriptComponent*);

		template<typename T> constexpr void Bind();
	};
}

#include "NativeScriptComponent.inl"
