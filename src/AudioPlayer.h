#ifndef LIBAUDIO_AUDIO_PLAYER_H
# define LIBAUDIO_AUDIO_PLAYER_H

#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>
#include <portaudio.h>
#include <cstdio>
#include <string>

namespace libaudio
{

	class AudioPlayer
	{

	private:
		float gain;
		bool loop;

	public:
		PaStreamParameters outputParameters;
		PaStream *stream;
		int16_t *datas;
		size_t len;
		size_t pos;
		AudioPlayer(char *datas, size_t len, int rate, int channelsCount);
		~AudioPlayer();
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
