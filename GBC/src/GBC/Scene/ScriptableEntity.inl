namespace gbc
{
	template<typename Component, typename... Args>
	Component& ScriptableEntity::Add(Args&&... args)
	{ return entity.Add<Component>(std::forward<Args>(args)...); }

	template<typename Component, typename... Args>
	Component& ScriptableEntity::AddOrReplace(Args&&... args)
	{ return entity.AddOrReplace<Component>(std::forward<Args>(args)...); }

	template<typename Component>
	Component& ScriptableEntity::AddOrGet()
	{ return entity.AddOrGet<Component>(); }

	template<typename Component>
	void ScriptableEntity::Remove()
	{ entity.Remove<Component>(); }

	template<typename Component>
	Component& ScriptableEntity::Get()
	{ return entity.Get<Component>(); }

	template<typename Component>
	const Component& ScriptableEntity::Get() const
	{ return entity.Get<Component>(); }

	template<typename Component>
	bool ScriptableEntity::Has() const
	{ return entity.Has<Component>(); }

	inline UUID ScriptableEntity::GetUUID() { return entity.GetUUID(); }
	inline std::string& ScriptableEntity::GetName() { return entity.GetName(); }
	inline const std::string& ScriptableEntity::GetName() const { return entity.GetName(); }
	inline TransformComponent& ScriptableEntity::GetTransform() { return entity.GetTransform(); }
	inline const TransformComponent& ScriptableEntity::GetTransform() const { return entity.GetTransform(); }
	inline glm::mat4 ScriptableEntity::GetAbsoluteTransform() const { return entity.GetAbsoluteTransform(); }

	inline bool ScriptableEntity::HasParent() const { return entity.HasParent(); }
	inline Entity ScriptableEntity::GetParent() const { return entity.GetParent(); }
	inline void ScriptableEntity::SetParent(Entity newParent) { entity.SetParent(newParent); }
	inline void ScriptableEntity::RemoveParent() { entity.RemoveParent(); }
	inline bool ScriptableEntity::HasChildren() const { return entity.HasChildren(); }
	inline Entity ScriptableEntity::GetFirstChild() const { return entity.GetFirstChild(); }
	inline Entity ScriptableEntity::GetNextSibling() const { return entity.GetNextSibling(); }
	inline Entity ScriptableEntity::GetPreviousSibling() const { return entity.GetPreviousSibling(); }
	inline bool ScriptableEntity::IsChildOf(Entity entity) const { return this->entity.IsChildOf(entity); }
	inline bool ScriptableEntity::IsSubChildOf(Entity entity) const { return this->entity.IsSubChildOf(entity); }
}
