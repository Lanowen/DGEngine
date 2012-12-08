#include "AudioSystem.h"

void AudioSystem::Play2DSound(std::string filename)
{
	FMOD::Sound* sound = this->Load2DSound(filename);
	FMOD::Channel* channel = this->Play(sound);
	channel->setChannelGroup(this->m_sfxGroup);
}

void AudioSystem::Play3DSound(std::string filename, GameObject* gameObject)
{
	FMOD_RESULT result;
	FMOD::Sound* sound = this->Load3DSound(filename);
	FMOD::Channel* channel = this->Play(sound);

	FMOD_VECTOR pos;

	pos.x = gameObject->position.x;
	pos.y = gameObject->position.y;
	pos.z = gameObject->position.z;

	result = channel->set3DAttributes(&pos, 0);
	channel->setChannelGroup(this->m_masterGroup);

	m_sound3DObjects[filename] = Sound3DObject(gameObject, sound, channel);
}

void AudioSystem::PlayMusic(std::string filename)
{
	FMOD::Sound* sound = this->Load2DSound(filename);
	FMOD::Channel* channel = this->Play(sound);
	channel->setChannelGroup(this->m_musicGroup);
}

FMOD::Channel* AudioSystem::Play(FMOD::Sound* sound)
{
	FMOD::Channel* channel;
	FMOD_RESULT result;
	result = this->m_fmod->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);

	return channel;
}

FMOD::Sound* AudioSystem::Load2DSound(std::string filename)
{
	if (this->m_sounds2d.find(filename) == this->m_sounds2d.end())
	{
		this->m_sounds2d[filename] = this->LoadFile(filename, FMOD_DEFAULT);
	}
	
	return this->m_sounds2d[filename];
}

FMOD::Sound* AudioSystem::Load3DSound(std::string filename)
{
	if (this->m_sounds3d.find(filename) == this->m_sounds3d.end())
	{
		this->m_sounds3d[filename] = this->LoadFile(filename, FMOD_3D);
	}
	
	return this->m_sounds3d[filename];
}

FMOD::Sound* AudioSystem::LoadFile(std::string filename, FMOD_MODE mode)
{

	FMOD::Sound* sound;

	FMOD_RESULT result;

	result = this->m_fmod->createSound(filename.c_str(), (FMOD_MODE)(FMOD_HARDWARE | mode), 0, &sound);
	if(result != FMOD_OK){
		return 0;
	}

	return sound;
}

void AudioSystem::SetListener(GameObject* gameObject)
{
	this->m_listener = gameObject;
	
	FMOD_VECTOR pos;
	FMOD_VECTOR forward;
	FMOD_VECTOR up;

	up.x = 0; up.y = 1; up.z = 0;

	forward.x = gameObject->forward.x;
	forward.y = gameObject->forward.y;
	forward.z = gameObject->forward.z;
}

void AudioSystem::Update(double time)
{

	FMOD_VECTOR pos;
	FMOD_VECTOR vel;
	FMOD_VECTOR forward;
	FMOD_VECTOR up;
	FMOD_RESULT result;

	up.x = 0; up.y = 1; up.z = 0;

	forward.x = m_listener->forward.x;
	forward.y = m_listener->forward.y;
	forward.z = m_listener->forward.z;

	pos.x = this->m_listener->position.x;
	pos.y = this->m_listener->position.y;
	pos.z = this->m_listener->position.z;

	vel.x = (pos.x - this->m_listenerLastPos.x)/time;
	vel.y = (pos.x - this->m_listenerLastPos.y)/time;
	vel.z = (pos.x - this->m_listenerLastPos.z)/time;

	result = this->m_fmod->set3DListenerAttributes(0, &pos, &vel, &forward, &up);

	this->m_fmod->update();

	this->m_listenerLastPos = pos;

	for each(std::pair<std::string, Sound3DObject> sound3dobj in m_sound3DObjects){
		Sound3DObject obj = sound3dobj.second;

		pos.x = obj.source->position.x;
		pos.y = obj.source->position.y;
		pos.z = obj.source->position.z;

		result = obj.channel->set3DAttributes(&pos, 0);
	}
}

bool AudioSystem::Initialize()
{
	FMOD_RESULT result;
	unsigned int version;
	int numDrivers;
	FMOD_CAPS caps;
	char name[256];

	result = FMOD::System_Create(&this->m_fmod);
	this->CheckResult(result);

	result = this->m_fmod->getVersion(&version);
	this->CheckResult(result);

	if (version < FMOD_VERSION)
	{
		return false;
	}

	result = this->m_fmod->getNumDrivers(&numDrivers);
	this->CheckResult(result);

	if (numDrivers == 0)
	{
		result = this->m_fmod->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		this->CheckResult(result);
	}
	else
	{
		result = this->m_fmod->getDriverCaps(0, &caps, 0, &this->m_speakermode);
		this->CheckResult(result);

		result = this->m_fmod->setSpeakerMode(this->m_speakermode);
		this->CheckResult(result);

		if (caps && FMOD_CAPS_HARDWARE_EMULATED)
		{
			result = this->m_fmod->setDSPBufferSize(1024, 10);
			this->CheckResult(result);
		}

		result = this->m_fmod->getDriverInfo(0, name, 256, 0);
		this->CheckResult(result);

		if (strstr(name, "SigmaTel"))
		{
			result = this->m_fmod->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			this->CheckResult(result);
		}
	}

	result = this->m_fmod->init(100, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		result = this->m_fmod->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		this->CheckResult(result);

		result = this->m_fmod->init(100, FMOD_INIT_NORMAL, 0);
	}
	this->CheckResult(result);

	this->m_fmod->createChannelGroup("MasterChannelGroup", &this->m_masterGroup);
	this->m_fmod->getMasterChannelGroup(&this->m_masterGroup);

	this->m_fmod->createChannelGroup("SFXGroup", &this->m_sfxGroup);
	this->m_fmod->createChannelGroup("MusicGroup", &this->m_musicGroup);

	this->m_ambientVolume = 0.15f;
	this->m_soundsVolume = 0.15f;
	this->m_musicVolume = 0.15f;

	this->m_masterGroup->setVolume(this->m_soundsVolume);
	this->m_sfxGroup->setVolume(this->m_ambientVolume);
	this->m_musicGroup->setVolume(this->m_musicVolume);

	this->m_fmod->set3DSettings(1.0f, 1.0f, 0.5f);

	return true;
}

void AudioSystem::CheckResult(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		Logging::ThrowError(L"Could not initialize FMOD.");
	}
}

void AudioSystem::Shutdown()
{
	this->m_fmod->release();
	this->m_listener = 0;
}

AudioSystem::AudioSystem()
{
	this->m_listener = 0;
}

AudioSystem::AudioSystem(const AudioSystem&)
{
}

AudioSystem::~AudioSystem()
{
}