#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "IComponent.h"

class IRenderComponent : public IComponent
{
public:
	typedef std::shared_ptr<IRenderComponent> Ptr;
	typedef std::weak_ptr<IRenderComponent> WeakPtr;

	IRenderComponent(GameObject &gameObject, RenderWindow& renderWindow)
		: IComponent(gameObject)
		, m_renderWindow(renderWindow)
	{}

	virtual void VDraw() = 0;

protected:
	RenderWindow& m_renderWindow;
};