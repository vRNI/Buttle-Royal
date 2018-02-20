#include "stdafx.h"

#include "TransformAnimationComponent.h"

#include "GameObject.h"

TransformAnimationComponent::TransformAnimationComponent(
									GameObject& gameObject,
									Transform& transform,
									ITransformAnimation::Ptr animation)
: IAnimationComponent(gameObject)
, m_transform(transform)
, m_animation(animation)
{
}

bool 
TransformAnimationComponent::VInit()
{
	if (m_animation == nullptr)
	{
		FF_ERROR_MSG("TransformAnimation must not be nullptr")
		return false;
	}
	return true;
}

void 
TransformAnimationComponent::VUpdate(float fDeltaTime)
{
	FF_ASSERT_MSG(m_animation != nullptr, "Animation must not be nullptr");

	m_animation->VUpdate(fDeltaTime, m_transform);
}




