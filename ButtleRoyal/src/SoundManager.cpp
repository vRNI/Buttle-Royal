#include "stdafx.h"

#include "InputManager.h"
#include "SoundManager.h"

SoundManager& SoundManager::getInstance()
{
	static SoundManager instance;
	return instance;
}
void SoundManager::PlaySound(std::string id)
{
	SoundBuffer* buffer = getSoundBuffer(id);

	Sound* sound = new Sound();
	sound->setBuffer(*buffer);
	sound->play();
	//delete buffer;
	//delete sound;
}

SoundManager::SoundManager():m_soundBuffer()
{
}

void SoundManager::RegisterSoundBuffer(std::string id, std::string fileName)
{
	SoundBuffer* buffer = new SoundBuffer();
	buffer->loadFromFile(fileName);
	m_soundBuffer[id]=buffer;

}

SoundBuffer* SoundManager::getSoundBuffer(std::string id)
{
	const auto it = m_soundBuffer.find(id);
	if (it != m_soundBuffer.end())
	{
		return it->second;
	}
}
void SoundManager::Clear()
{
	//m_soundBuffer.clear();
	m_sounds.clear();
}






