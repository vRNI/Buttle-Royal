#pragma once

#include "IComponent.h"

class IAnimationComponent : public IComponent
{
public:
	typedef std::shared_ptr<IAnimationComponent> Ptr;

	using IComponent::IComponent;
};