#ifndef LIBAUDIO_CACHED_AUDIO_PLAYER_H
# define LIBAUDIO_CACHED_AUDIO_PLAYER_H

#include "AudioPlayer.h"
#include <cstdint>

namespace libaudio
{

	class CachedAudioPlayer : public AudioPlayer
	{

	private:
		int16_t *datas;
		size_t len;
		size_t pos;
		static int callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *paTimeInfo, PaStreamCallbackFlags statusFlags, void *userData);

	public:
		CachedAudioPlayer(int16_t *datas, size_t len, int rate, int channelsCount);
		void stop();

	};

}

#endif
