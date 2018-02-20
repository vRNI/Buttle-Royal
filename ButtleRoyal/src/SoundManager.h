#pragma once

#include "SFML/Audio/Sound.hpp"

class SoundManager
{
public:

	static SoundManager& getInstance();

	void RegisterSoundBuffer(std::string id, std::string fileName);
	SoundBuffer* getSoundBuffer(std::string id);
	void PlaySound(std::string name);
	void Clear();
private:
	SoundManager();
	~SoundManager() = default;

	SoundManager(const SoundManager& rhv) = delete;
	SoundManager& operator= (const SoundManager& rhv) = delete;

	std::map<std::string, Sound*> m_sounds;
	std::map<std::string, SoundBuffer*> m_soundBuffer;

};
