#pragma once

namespace gbc
{
	class ScriptableEntity;

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity*(*createFunc)();
		void(*destroyFunc)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			createFunc = []() { return static_cast<ScriptableEntity*>(new T()); };
			destroyFunc = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}
	};
}
