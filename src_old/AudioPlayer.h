#ifndef AUDIO_PLAYER_H
# define AUDIO_PLAYER_H

# include <sndfile.h>
# include <AL/alc.h>
# include <AL/al.h>
# include <string>

namespace libaudio
{

	class AudioPlayer
	{

	private:
		ALuint buffer;
		ALuint source;

	public:
		AudioPlayer(std::string filename);
		~AudioPlayer();
		bool play();
		bool pause();
		bool setGain(float gain);
		bool setPitch(float pitch);
		bool setLooping(bool looping);

	};

}

#endif
