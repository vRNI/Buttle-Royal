#include "stdafx.h"

#include "InputManager.h"

InputManager& InputManager::getInstance()
{
	static InputManager instance;
	return instance;
}

void
InputManager::process(const Event &event)
{
	switch (event.type)
	{
	case Event::KeyPressed:
		eventFrame.keys[event.key.code] = true;
		break;
	case Event::KeyReleased:
		eventFrame.keys[event.key.code] = false;
		break;
	case Event::JoystickButtonPressed:

		eventFrame.buttons[event.joystickButton.joystickId][event.joystickButton.button] = true;
		break;
	case Event::JoystickButtonReleased:
		eventFrame.buttons[event.joystickButton.joystickId][event.joystickButton.button] = false;
		break;
	default:
		//nothing happens
		break;
	}
}

void
InputManager::update()
{
	lastFrame = currentFrame;
	currentFrame = eventFrame;
}

void InputManager::bindController(int player, int controller)
{
	FF_ASSERT_MSG(player < PLAYER_COUNT, "player out of bounds");
	FF_ASSERT_MSG(controller < PLAYER_COUNT, "controller out of bounds");
	bool ok = false;
	for (auto it = controllerMappings->begin(); it != controllerMappings->end(); ++it)
	{
		if (it->second == controller)
			ok = true;
	}
	if (!ok)
		controllerMappings->insert(std::make_pair(player, controller));
}

void
InputManager::bind(const std::string& action, int keyCode, int playerIdx, bool controller)
{
	FF_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");

	if (!controller)
		actionBinding[playerIdx][action] = keyCode;
	else
	{
		if (controllerActionBinding.size() < 8)
		{
			controllerActionBinding.resize(8);
		}
		controllerActionBinding[playerIdx].insert({ action,keyCode });
	}
}

void
InputManager::unbind(const std::string& action, int playerIdx, bool controller)
{
	FF_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");
	if (!controller)
		actionBinding[playerIdx].erase(action);
	else
		controllerActionBinding[playerIdx].erase(action);
}

int
InputManager::getKeyForAction(const std::string& action, int playerIdx)
{
	FF_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");

	const auto it = actionBinding[playerIdx].find(action);
	if (it != actionBinding[playerIdx].end())
	{
		return it->second;
	}
	return 0;
}
int
InputManager::getButtonForAction(const std::string& action, int playerIdx)
{
	FF_ASSERT_MSG(playerIdx < PLAYER_COUNT, "player out of bounds");

	auto it = controllerActionBinding[playerIdx].find(action);
	if (it != controllerActionBinding[playerIdx].end())
	{
		return it->second;
	}
	return 20;
}
bool
InputManager::isKeyDown(const std::string& action, int player)
{
	return isKeyDown(getKeyForAction(action, player));
}

bool
InputManager::isKeyUp(const std::string& action, int player)
{
	return isKeyUp(getKeyForAction(action, player));
}

bool
InputManager::isKeyPressed(const std::string& action, int player)
{
	return isKeyPressed(getKeyForAction(action, player));
}

bool
InputManager::isKeyReleased(const std::string& action, int playerIdx)
{
	return isKeyReleased(getKeyForAction(action, playerIdx));
}

bool
InputManager::isButtonDown(const std::string& action, int playerIdx)
{
	return isButtonDown(getButtonForAction(action, playerIdx), playerIdx);

}
bool
InputManager::isButtonUp(const std::string& action, int playerIdx)
{
	return isButtonUp(getButtonForAction(action, playerIdx), playerIdx);
}
bool
InputManager::isButtonPressed(const std::string& action, int playerIdx)
{
	return isButtonPressed(getButtonForAction(action, playerIdx), playerIdx);
}
bool
InputManager::isButtonReleased(const std::string& action, int playerIdx)
{
	return isButtonReleased(getButtonForAction(action, playerIdx), playerIdx);
}

Vector2f
InputManager::getMousePosition()
{
	FF_ASSERT_MSG(renderWindow != nullptr, "RenderWindow not set for getMousePosition.");

	Vector2i p = Mouse::getPosition(*renderWindow);
	return Vector2f((float)(p.x), (float)(p.y));
}

Vector2f InputManager::getAxixPosition(int playerIdx)
{
	auto it = controllerMappings->find(playerIdx);
	if (it == controllerMappings->end()) //if no controller bound add no input
		return Vector2f(0, 0);

	Vector2f temp;
	temp.x = Joystick::getAxisPosition(it->second, Joystick::X);
	temp.y = Joystick::getAxisPosition(it->second, Joystick::Y);

	if (temp.x < 30 && temp.x > -30) temp.x = 0;
	if (temp.y < 30 && temp.y > -30) temp.y = 0;

	return temp;
}
std::vector<int> InputManager::GetControllers()
{
	Joystick::update();
	std::vector<int> controllers;
	for (int i = 0; i<9; ++i)
	{
		if (Joystick::isConnected(i))
			controllers.push_back(i);
	}
	return controllers;
}