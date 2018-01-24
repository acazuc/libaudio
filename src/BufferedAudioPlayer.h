#ifndef LIBAUDIO_BUFFERED_AUDIO_PLAYER_H
# define LIBAUDIO_BUFFERED_AUDIO_PLAYER_H

# include "AudioPlayer.h"
# include <vorbis/vorbisfile.h>
# include <vorbis/codec.h>
# include <cstdio>
# include <string>


namespace libaudio
{

	class BufferedAudioPlayer : public AudioPlayer
	{

	private:
		OggVorbis_File vorbisFile;
		vorbis_info *vorbisInfos;
		FILE *file;
		static int callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *paTimeInfo, PaStreamCallbackFlags statusFlags, void *userData);

	public:
		BufferedAudioPlayer(std::string file);
		~BufferedAudioPlayer();
		void stop();

	};

}

#endif
