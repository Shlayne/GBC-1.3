namespace gbc
{
	template<typename T, typename... Args>
	T* EditorLayer::AddPanel(const std::string& name, Args&&... args)
	{
		T* panel = new T(name, std::forward<Args>(args)...);
		panels[name] = panel;
		return panel;
	}
}
