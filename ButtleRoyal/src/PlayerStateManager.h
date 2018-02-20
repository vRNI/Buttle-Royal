#pragma once

#include "stdafx.h"
#include "IPlayerState.h"

class PlayerStateManager : public IComponent
{
public:
	typedef std::shared_ptr<PlayerStateManager> Ptr;

	PlayerStateManager(GameObject& player);
	~PlayerStateManager(){};

	void VUpdate(float fDeltaTime) override;
	bool VInit() override;

	/// registers a new GameState with the given name.
	void registerState(const std::string& name, IPlayerState::Ptr playerState);

	/// Changes to the given state
	void setState(const std::string& stateName);
	std::string getCurrentStateName();

	/// update VHandleinput of the current state is called here
	/// if current state wants to change it returns string here
	void update(float delta){};
	void draw();

private:
	IPlayerState* findState(const std::string& stateName);
	void changeState(IPlayerState* stateName);

	std::map<std::string, IPlayerState::Ptr> m_states;
	IPlayerState* currentState = nullptr;
	IPlayerState* futureState = nullptr;
};
