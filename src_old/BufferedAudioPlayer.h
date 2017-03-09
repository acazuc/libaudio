#ifndef BUFFERED_AUDIO_PLAYER_H
# define BUFFERED_AUDIO_PLAYER_H

# define BUFFERED_AUDIO_PLAYER_BUFFER_NUMBER 10

# include <sndfile.h>
# include <AL/alc.h>
# include <AL/al.h>
# include <string>

namespace libaudio
{

	class BufferedAudioPlayer
	{

	private:
		SNDFILE *sndfile;
		ALsizei sampleRate;
		ALsizei nbSamples;
		ALuint buffers[BUFFERED_AUDIO_PLAYER_BUFFER_NUMBER];
		ALenum format;
		ALuint source;
		float oggMax;
		bool buffersCreated;
		bool sourceCreated;
		bool looping;
		bool ended;
		bool fillBuffer(ALuint buff);

	public:
		BufferedAudioPlayer(std::string filename);
		~BufferedAudioPlayer();
		bool tick();
		bool play();
		bool pause();
		bool setGain(float gain);
		bool setPitch(float pitch);
		void setLooping(bool looping);

	};

}

#endif
