#pragma once

#include "CameraComponent.h"
#include "CircleRendererComponent.h"
#include "IDComponent.h"
#include "Model3DComponent.h"
#include "NativeScriptComponent.h"
#include "RelationshipComponent.h"
#include "SpriteRendererComponent.h"
#include "TagComponent.h"
#include "TransformComponent.h"
#include "Physics/BoxCollider2DComponent.h"
#include "Physics/Rigidbody2DComponent.h"

namespace gbc
{
	template<typename... Component>
	struct ComponentGroup{};

	// Doesn't have IDComponent, RelationshipComponent, nor TagComponent
	using AllCopyableComponents = ComponentGroup
	<
		CameraComponent,
		CircleRendererComponent,
		Model3DComponent,
		NativeScriptComponent,
		SpriteRendererComponent,
		TransformComponent,
		BoxCollider2DComponent,
		Rigidbody2DComponent
	>;
	
	using AllComponents = ComponentGroup
	<
		CameraComponent,
		CircleRendererComponent,
		IDComponent,
		Model3DComponent,
		NativeScriptComponent,
		RelationshipComponent,
		SpriteRendererComponent,
		TagComponent,
		TransformComponent,
		BoxCollider2DComponent,
		Rigidbody2DComponent
	>;
}
