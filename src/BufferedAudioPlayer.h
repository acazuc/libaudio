#ifndef LIBAUDIO_BUFFERED_AUDIO_PLAYER_H
# define LIBAUDIO_BUFFERED_AUDIO_PLAYER_H

#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>
#include <portaudio.h>
#include <cstdio>
#include <string>

namespace libaudio
{

	class BufferedAudioPlayer
	{

	private:
		FILE *file;
		float gain;
		bool loop;

	public:
		PaStreamParameters outputParameters;
		OggVorbis_File vorbisFile;
		vorbis_info *vorbisInfos;
		PaStream *stream;
		BufferedAudioPlayer(std::string file);
		~BufferedAudioPlayer();
		void play();
		void pause();
		void stop();
		inline float getGain() {return (this->gain);};
		inline void setGain(float gain) {this->gain = std::min(1.f, std::max(0.f, gain));};
		inline bool isLoop() {return (this->loop);};
		inline void setLoop(bool loop) {this->loop = loop;};

	};

}

#endif
