#ifndef LIBAUDIO_AUDIO_PLAYER_H
# define LIBAUDIO_AUDIO_PLAYER_H

#include <portaudio.h>
#include <algorithm>

namespace libaudio
{

	class AudioPlayer
	{

	protected:
		float gain;
		bool loop;

	public:
		PaStreamParameters outputParameters;
		PaStream *stream;
		AudioPlayer();
		~AudioPlayer();
		void play();
		void pause();
		void stop();
		bool isPlaying();
		inline float getGain() {return (this->gain);};
		inline void setGain(float gain) {this->gain = std::min(1.f, std::max(0.f, gain));};
		inline bool isLoop() {return (this->loop);};
		inline void setLoop(bool loop) {this->loop = loop;};
	};

};

#endif
