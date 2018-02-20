#pragma once

#include <unordered_map>

#include "SFML/Window.hpp"

class InputManager
{
public:

	static InputManager& getInstance();

	/// Must be called at beginning of game update, before the update
	/// method. The events are
	/// classified and processed internally.
	/// \param event a single event coming from the sf polling.
	void process(const Event &event);

	/// Must be called after all events have been processed.
	void update();

	/// Binds an action to the given keycode and for the given player
	/// \param action the name of the action (eg. left, jump, ...)
	/// \param keyCode the keycode of the action
	/// \param playerIdx the idx of the player
	void bind(const std::string& action, int keyCode, int playerIdx = 0, bool controller = false);

	void bindController(int controller, int playerIdx);
	// void unbindController(int controller);

	/// Unbinds an action.
	void unbind(const std::string& action, int playerIdx = 0, bool controller = false);

	/// Returns true if the key button is currently down.
	bool isKeyDown(int keyCode)
	{
		FF_ASSERT_MSG(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return currentFrame.keys[keyCode];
	}

	/// Returns true if the key button is currently up.
	bool isKeyUp(int keyCode)
	{
		FF_ASSERT_MSG(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return !currentFrame.keys[keyCode];
	}

	/// Returns true if the key button has been pressed.
	bool isKeyPressed(int keyCode)
	{
		FF_ASSERT_MSG(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return currentFrame.keys[keyCode] && !lastFrame.keys[keyCode];
	}



	/// Returns true if the key button has been released.
	bool isKeyReleased(int keyCode)
	{
		FF_ASSERT_MSG(keyCode >= 0 && keyCode < sf::Keyboard::KeyCount, "KeyCode out of bounds");
		return !currentFrame.keys[keyCode] && lastFrame.keys[keyCode];
	}

	bool isButtonUp(int button, int playerIdx)
	{
		FF_ASSERT_MSG(button >= 0 && button < sf::Joystick::ButtonCount, "Button out of bounds");
		return !currentFrame.buttons[playerIdx][button];
	}

	bool isButtonDown(int button, int playerIdx)
	{
		FF_ASSERT_MSG(button >= 0 && button < sf::Joystick::ButtonCount, "Button out of bounds");
		return currentFrame.buttons[playerIdx][button];
	}

	bool isButtonPressed(int button, int playerIdx)
	{
		FF_ASSERT_MSG(button >= 0 && button < sf::Joystick::ButtonCount, "Button out of bounds");
		return currentFrame.buttons[playerIdx][button] && !lastFrame.buttons[playerIdx][button];
	}

	bool isButtonReleased(int button, int playerIdx)
	{
		FF_ASSERT_MSG(button >= 0 && button <  sf::Joystick::ButtonCount, "Button out of bounds");
		return !currentFrame.buttons[playerIdx][button] && lastFrame.buttons[button];
	}

	/// Returns true if the button for the given Action is currently down.
	bool isKeyDown(const std::string& action, int playerIdx = 0);
	bool isButtonDown(const std::string& action, int playerIdx = 0);
	/// Returns true if the button for the given Action is currently up.
	bool isKeyUp(const std::string& action, int playerIdx = 0);
	bool isButtonUp(const std::string& action, int playerIdx = 0);
	/// Returns true if the button for the given Action has been pressed.
	bool isKeyPressed(const std::string& action, int playerIdx = 0);
	bool isButtonPressed(const std::string& action, int playerIdx = 0);
	/// Returns true if the button for the given Action has been released.
	bool isKeyReleased(const std::string& action, int playerIdx = 0);
	bool isButtonReleased(const std::string& action, int playerIdx = 0);
	/// Returns the current mouse position relative to the window
	Vector2f getMousePosition();

	Vector2f getAxixPosition(int playerIdx);
	std::vector<int> GetControllers();
	void setRenderWindow(RenderWindow* window)
	{
		renderWindow = window;
	}

private:
	InputManager() = default;
	~InputManager() = default;

	InputManager(const InputManager& rhv) = delete;
	InputManager& operator= (const InputManager& rhv) = delete;

	int getKeyForAction(const std::string& action, int playerIdx);
	int getButtonForAction(const std::string& action, int playerIdx);

	struct FrameData
	{
		bool keys[Keyboard::KeyCount];
		bool buttons[8][Joystick::ButtonCount];
	};
	FrameData lastFrame{};
	FrameData currentFrame{};
	FrameData eventFrame{};

	RenderWindow* renderWindow;

	/// maximum allowed players. Can be increaded if needed.
	static const int PLAYER_COUNT = 8;
	std::map<int, int> controllerMappings[PLAYER_COUNT];
	std::map<std::string, int> actionBinding[PLAYER_COUNT];

	std::vector<std::map<std::string, int>> controllerActionBinding;

};
