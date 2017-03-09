#include "AudioPlayer.h"
#include "Exception.h"
#include <iostream>
#include <cstring>
#include <vector>

namespace libaudio
{

	AudioPlayer::AudioPlayer(std::string filename)
	{
		SF_INFO fileInfos;
		SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &fileInfos);
		if (!file)
			throw Exception("Can't open file");
		ALsizei nbSamples  = static_cast<ALsizei>(fileInfos.channels * fileInfos.frames);
		ALsizei sampleRate = static_cast<ALsizei>(fileInfos.samplerate);
		if (nbSamples <= 0)
		{
			sf_close(file);
			throw Exception("Invalid samples number");
		}
		float *raw = new float[nbSamples];
		sf_read_float(file, raw, nbSamples);
		sf_close(file);
		ALshort *samples = new ALshort[nbSamples];
		int16_t *d = &samples[0];
		float m_oggmax = 1;
		for (int64_t i = 0; i < nbSamples; i++)
		{
			if (raw[i] > m_oggmax)
				m_oggmax = raw[i];
			d[i] = raw[i] / m_oggmax * 32767.0f;
		}
		delete[] (raw);
		ALenum format;
		switch (fileInfos.channels)
		{
			case 1:
				format = AL_FORMAT_MONO16;
				break;
			case 2:
				format = AL_FORMAT_STEREO16;
				break;
			default:
			{
				delete[] (samples);
				throw Exception("Invalid channels");
			}
		}
		alGenSources(1, &this->source);
		if (alGetError() != AL_NO_ERROR)
			throw Exception("Can't generate OpenAL source");
		alGenBuffers(1, &this->buffer);
		if (alGetError() != AL_NO_ERROR)
		{
			delete[] (samples);
			alDeleteSources(1, &this->source);
			throw Exception("Can't generate OpenAL buffer");
		}
		alBufferData(this->buffer, format, &samples[0], nbSamples * sizeof(int16_t), sampleRate);
		delete[] (samples);
		if (alGetError() != AL_NO_ERROR)
		{
			alDeleteBuffers(1, &this->buffer);
			alDeleteSources(1, &this->source);
			throw Exception("Cant't fill OpenAL buffer datas");
		}
		alSourcei(this->source, AL_BUFFER, this->buffer);
		if (alGetError() != AL_NO_ERROR)
		{
			alDeleteBuffers(1, &this->buffer);
			alDeleteSources(1, &this->source);
			throw Exception("Can't bind OpenAL buffer to OpenAL source");
		}
	}

	AudioPlayer::~AudioPlayer()
	{
		alDeleteBuffers(1, &this->buffer);
		alDeleteSources(1, &this->source);
	}

	bool AudioPlayer::play()
	{
		alSourcePlay(this->source);
		return (alGetError() == AL_NO_ERROR);
	}

	bool AudioPlayer::pause()
	{
		alSourcePause(this->source);
		return (alGetError() == AL_NO_ERROR);
	}

	bool AudioPlayer::setGain(float gain)
	{
		alSourcef(this->source, AL_GAIN, gain);
		return (alGetError() == AL_NO_ERROR);
	}

	bool AudioPlayer::setPitch(float pitch)
	{
		alSourcef(this->source, AL_PITCH, pitch);
		return (alGetError() == AL_NO_ERROR);
	}

	bool AudioPlayer::setLooping(bool looping)
	{
		alSourcei(this->source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
		return (alGetError() == AL_NO_ERROR);
	}

}
