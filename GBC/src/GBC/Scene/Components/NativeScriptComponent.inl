namespace gbc
{
	template<typename T>
	void NativeScriptComponent::Bind()
	{
		createFunc = []() { return static_cast<ScriptableEntity*>(new T()); };
		destroyFunc = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
	}
}
