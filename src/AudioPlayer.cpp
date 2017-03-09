#include "AudioPlayer.h"

namespace libaudio
{

	AudioPlayer::AudioPlayer()
	: gain(1)
	, loop(false)
	{
		//Empty
	}

	AudioPlayer::~AudioPlayer()
	{
		Pa_CloseStream(&this->stream);
	}

	void AudioPlayer::play()
	{
		 Pa_StartStream(this->stream);
	}

	void AudioPlayer::pause()
	{
		Pa_AbortStream(this->stream);
	}

	void AudioPlayer::stop()
	{
		Pa_AbortStream(this->stream);
	}

}
