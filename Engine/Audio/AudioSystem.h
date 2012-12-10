#pragma once

#include <string>
#include <map>
#include <vector>

#include <fmod.hpp>

#include <GameObject.hpp>
#include <Utils\Logging.h>
#include <map>
#include <EMath.hpp>

struct Sound3DObject {
public:
	GameObject* source;
	FMOD::Sound* sound;
	FMOD::Channel* channel;
	FMOD_VECTOR listenerLastPos;

	Sound3DObject() : source(0), sound(0), channel(0) {}

	Sound3DObject(GameObject* source, FMOD::Sound* sound, FMOD::Channel* channel, FMOD_VECTOR prevPos): source(source), sound(sound), channel(channel), listenerLastPos(prevPos) {}

	Sound3DObject(const Sound3DObject& obj): source(obj.source), sound(obj.sound), channel(obj.channel), listenerLastPos(obj.listenerLastPos) {}
};

class AudioSystem{
private:
	typedef std::map<std::string, FMOD::Sound*> SoundMap;
	typedef std::map<std::string, Sound3DObject> Sound3DObjectMap;
	
public:
	AudioSystem();
	AudioSystem(const AudioSystem&);
	~AudioSystem();

	void Update(double);

	void Play2DSound(std::string);
	void Play3DSound(std::string, GameObject*);
	void PlayMusic(std::string);

	void SetListener(GameObject*);

private:
	void CheckResult(FMOD_RESULT);
	FMOD::Sound* Load2DSound(std::string);
	FMOD::Sound* Load3DSound(std::string);
	FMOD::Sound* LoadFile(std::string, FMOD_MODE);
	FMOD::Channel* Play(FMOD::Sound*);

private:
	FMOD::System* m_fmod;
	FMOD_SPEAKERMODE m_speakermode;
	SoundMap m_sounds3d;
	SoundMap m_sounds2d;
	float m_ambientVolume;
	float m_soundsVolume;
	float m_musicVolume;

	GameObject* m_listener;
	FMOD_VECTOR m_listenerLastPos;

	FMOD::ChannelGroup *m_masterGroup, *m_sfxGroup, *m_musicGroup;

	
	Sound3DObjectMap m_sound3DObjects;
};