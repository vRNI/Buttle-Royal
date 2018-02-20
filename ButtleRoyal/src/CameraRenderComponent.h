#pragma once

#include <SFML/Graphics.hpp>

#include "IRenderComponent.h"

/// Updates the camera/view of the renderwindow. This new camera
/// will be used until another CameraRenderComponent changes it.
/// For positioning the translation component of the gameobject 
/// transform is used.
/// Currently does not support rotation.
class CameraRenderComponent : public IRenderComponent
{
public:
	typedef std::shared_ptr<CameraRenderComponent> Ptr;

	/// \param view the camera specifications.
	CameraRenderComponent(GameObject &gameObject,
						  RenderWindow &renderWindow,
						  sf::View view);

	virtual bool VInit() override;
	virtual void VUpdate(float fDeltaTime) override;
	virtual void VDraw() override;

private:
	/// the current camera specifications
	sf::View m_view;
	sf::Vector2f m_InitialCenter;
};