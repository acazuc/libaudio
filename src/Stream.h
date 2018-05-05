#ifndef LIBAUDIO_STREAM_H
# define LIBAUDIO_STREAM_H

# include "./Player.h"
# include <portaudio.h>
# include <vector>

namespace libaudio
{

	class Stream
	{

	private:
		std::vector<Player*> players;
		PaStreamParameters outputParameters;
		PaStream *stream;
		uint32_t rate;
		float pitch;
		float gain;
		float pan;
		static int callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *paTimeInfo, PaStreamCallbackFlags statusFlags, void *userData);

	public:
		Stream();
		~Stream();
		void start();
		void stop();
		void setPitch(float pitch);
		void setGain(float gain);
		void setPan(float pan);
		void addPlayer(Player *player);
		void removePlayer(Player *player);

	};

}

#endif
