namespace gbc
{
	template<typename T>
	T* EditorLayer::AddPanel(const std::string& name)
	{
		// emplace_back doesn't compile for whatever templatey reasons
		T* panel = new T(name, this);
		panels.push_back(panel);
		return panel;
	}
}
