#pragma once

#include <string>
#include <map>
#include <vector>

#include <fmod.hpp>

#include <GameObject.hpp>
#include <Utils\Logging.h>
#include <map>

struct Sound3DObject {
public:
	GameObject* source;
	FMOD::Sound* sound;
	FMOD::Channel* channel;

	Sound3DObject() : source(0), sound(0), channel(0) {}

	Sound3DObject(GameObject* source, FMOD::Sound* sound, FMOD::Channel* channel): source(source), sound(sound), channel(channel) {}

	Sound3DObject(const Sound3DObject& obj): source(obj.source), sound(obj.sound), channel(obj.channel) {}
};

class AudioSystem{
private:
	typedef std::map<std::string, FMOD::Sound*> SoundMap;
	typedef std::map<std::string, Sound3DObject> Sound3DObjectMap;
	
public:
	AudioSystem();
	AudioSystem(const AudioSystem&);
	~AudioSystem();

	bool Initialize();
	void Update(double);
	void Shutdown();

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