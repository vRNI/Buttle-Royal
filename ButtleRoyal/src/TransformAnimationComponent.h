#pragma once

#include "IAnimationComponent.h"

#include <SFML/Graphics.hpp>

/// Interface for specific animation implementations
class ITransformAnimation
{
public:
	typedef std::shared_ptr<ITransformAnimation> Ptr;

	virtual ~ITransformAnimation() = default;

	/// \param fDeltaTime
	/// \param transform the transform to be manipulated
	virtual void VUpdate(float fDeltaTime, Transform &transform) = 0;
};

/// The component controlling transform animations. The specific
/// animation pattern can be handed in via an implementation of
/// TransformAnimation. 
class TransformAnimationComponent : public IAnimationComponent
{
public:

	///
	typedef std::shared_ptr<TransformAnimationComponent> Ptr;

	/// \param gameObject the game object holding this component
	/// \param transform the transformation component, which is modified
	/// by this component
	TransformAnimationComponent(GameObject& gameObject,
								Transform& transform,
								ITransformAnimation::Ptr animation);

	virtual bool VInit() override;
	virtual void VUpdate(float fDeltaTime) override;

private:
	Transform& m_transform;

	ITransformAnimation::Ptr m_animation;
};