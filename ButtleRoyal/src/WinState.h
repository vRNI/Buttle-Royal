#pragma once

#include "GameState.h"

#include <SFML/Graphics/Text.hpp>
#include <TGUI/Gui.hpp>

class WinState : public GameState
{
public:
	using GameState::GameState;

	virtual void VInit() override;
	
	virtual void VUpdate(float delta) override;
	virtual void VDraw() override;
	void VHandleEvent(Event event) override;

private:
	// should be gameobject too
	Text m_text;
	Font m_font;
	View m_view;
	String m_loser;
	tgui::Gui m_gui;

	bool m_isInit = false;
};
