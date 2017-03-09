#ifndef LIBAUDIO_CACHED_AUDIO_PLAYER_H
# define LIBAUDIO_CACHED_AUDIO_PLAYER_H

#include "AudioPlayer.h"

#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>

namespace libaudio
{

	class CachedAudioPlayer : public AudioPlayer
	{

	private:

	public:
		int16_t *datas;
		size_t len;
		size_t pos;
		CachedAudioPlayer(char *datas, size_t len, int rate, int channelsCount);
		void stop();

	};

}

#endif
