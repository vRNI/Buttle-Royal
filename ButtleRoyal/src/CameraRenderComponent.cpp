#include "stdafx.h"

#include "GameObject.h"
#include "CameraRenderComponent.h"

CameraRenderComponent::CameraRenderComponent(
	GameObject &gameObject, 
	RenderWindow &renderWindow,
	View view)
: IRenderComponent(gameObject, renderWindow)
, m_view(view)
, m_InitialCenter(view.getCenter())
{
}

bool 
CameraRenderComponent::VInit()
{
	return true;
}

void
CameraRenderComponent::VDraw()
{
	
}

void
CameraRenderComponent::VUpdate(float fDeltaTime)
{
	const Vector2f newCenter = m_gameObject.getTransform().transformPoint(m_InitialCenter);
	m_view.setCenter(newCenter);

	View view = m_view;
	view.setCenter(floor(view.getCenter().x), floor(view.getCenter().y));
	m_renderWindow.setView(view);
}