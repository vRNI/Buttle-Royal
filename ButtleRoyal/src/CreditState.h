#pragma once

#include "GameState.h"

#include <SFML/Graphics/Text.hpp>
#include <TGUI/TGUI.hpp>

class CreditState : public GameState
{
public:
	using GameState::GameState;

	virtual void VInit() override;
	
	virtual void VUpdate(float delta) override;
	virtual void VDraw() override;
	virtual void VHandleEvent(Event event) override;

private:
	// should be gameobject too
	Text m_text;
	Font m_font;
	View m_view;
	tgui::Gui m_gui;
	tgui::Button::Ptr m_button;
	bool m_isInit = false;
	std::shared_ptr<tgui::Theme> m_theme;
	std::shared_ptr<tgui::Button> PlayButton;
	std::shared_ptr<tgui::Button> ControlsButton;
	std::shared_ptr<tgui::Button> CreditsButton;
};