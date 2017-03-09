#include "BufferedAudioPlayer.h"
# include "Exception.h"
#include <iostream>
#include <cstring>
#include <vector>

namespace libaudio
{

	BufferedAudioPlayer::BufferedAudioPlayer(std::string filename)
	: oggMax(1)
	, buffersCreated(false)
	, sourceCreated(false)
	, looping(false)
	, ended(false)
	{
		SF_INFO fileInfos;
		this->sndfile = sf_open(filename.c_str(), SFM_READ, &fileInfos);
		if (!this->sndfile)
			throw Exception("Cant open file");
		this->nbSamples  = static_cast<ALsizei>(fileInfos.channels * fileInfos.frames);
		this->sampleRate = static_cast<ALsizei>(fileInfos.samplerate);
		if (this->nbSamples <= 0)
		{
			sf_close(this->sndfile);
			throw Exception("Invalid samples number");
		}
		switch (fileInfos.channels)
		{
			case 1:
				this->format = AL_FORMAT_MONO16;
				break;
			case 2:
				this->format = AL_FORMAT_STEREO16;
				break;
			default:
				throw Exception("Invalid channels");
		}
		alGenSources(1, &this->source);
		if (alGetError() != AL_NO_ERROR)
			throw Exception("Can't generate OpenAL source");
		this->sourceCreated = true;
		alGenBuffers(BUFFERED_AUDIO_PLAYER_BUFFER_NUMBER, this->buffers);
		if (alGetError() != AL_NO_ERROR)
		{
			alDeleteSources(1, &this->source);
			throw Exception("Can't generate OpenAL buffers");
		}
		this->buffersCreated = true;
		for (uint8_t i = 0; i < BUFFERED_AUDIO_PLAYER_BUFFER_NUMBER; ++i)
		{
			if (fillBuffer(this->buffers[i]))
				alSourceQueueBuffers(this->source, 1, &this->buffers[i]);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			alDeleteSources(1, &this->source);
			alDeleteBuffers(BUFFERED_AUDIO_PLAYER_BUFFER_NUMBER, this->buffers);
			throw Exception("Can't queue OpenAL source buffers");
		}
	}

	BufferedAudioPlayer::~BufferedAudioPlayer()
	{
		if (this->sndfile)
			sf_close(this->sndfile);
		if (this->buffersCreated)
			alDeleteBuffers(BUFFERED_AUDIO_PLAYER_BUFFER_NUMBER, this->buffers);
		if (this->sourceCreated)
			alDeleteSources(1, &this->source);
	}

	bool BufferedAudioPlayer::tick()
	{
		ALint processed = 0;
		alGetSourcei(this->source, AL_BUFFERS_PROCESSED, &processed);
		while (processed--)
		{
			ALuint bufferId;
			alSourceUnqueueBuffers(this->source, 1, &bufferId);
			if (fillBuffer(bufferId))
				alSourceQueueBuffers(this->source, 1, &bufferId);
			else if (this->looping)
			{
				sf_seek(this->sndfile, 0, SEEK_SET);
				this->ended = false;
				if (fillBuffer(bufferId))
					alSourceQueueBuffers(this->source, 1, &bufferId);
				else
					return (false);
			}
			else
				return (false);
		}
		return (true);
	}

	bool BufferedAudioPlayer::fillBuffer(ALuint bufferId)
	{
		if (this->ended)
			return (false);
		float *raw = new float[this->sampleRate];
		sf_count_t readed;
		if ((readed = sf_read_float(this->sndfile, raw, this->sampleRate)) < this->sampleRate)
			this->ended = true;
		if (readed == 0)
		{
			delete[] (raw);
			return (false);
		}
		ALshort *samples = new ALshort[readed];
		for (int64_t i = 0; i < readed; ++i)
		{
			if (raw[i] > this->oggMax)
				this->oggMax = raw[i];
			samples[i] = raw[i] / this->oggMax * 32767.0f;
		}
		delete[] (raw);
		alBufferData(bufferId, this->format, samples, readed * sizeof(*samples), this->sampleRate);
		delete[] (samples);
		return (true);
	}

	bool BufferedAudioPlayer::play()
	{
		alSourcePlay(this->source);
		return (alGetError() == AL_NO_ERROR);
	}

	bool BufferedAudioPlayer::pause()
	{
		alSourcePause(this->source);
		return (alGetError() == AL_NO_ERROR);
	}

	bool BufferedAudioPlayer::setGain(float gain)
	{
		alSourcef(this->source, AL_GAIN, gain);
		return (alGetError() == AL_NO_ERROR);
	}

	bool BufferedAudioPlayer::setPitch(float pitch)
	{
		alSourcef(this->source, AL_PITCH, pitch);
		return (alGetError() == AL_NO_ERROR);
	}

	void BufferedAudioPlayer::setLooping(bool looping)
	{
		this->looping = looping;
	}

}
