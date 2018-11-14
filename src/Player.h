#ifndef LIBAUDIO_PLAYER_H
# define LIBAUDIO_PLAYER_H

# include <cstdint>

namespace libaudio
{


	class Player
	{

	friend class Stream;

	protected:
		uint32_t rate;
		float pitch;
		float gain;
		float pan;
		bool active;
		bool loop;
		void callback(float *out, unsigned long frameCount, uint32_t rate);
		virtual void getData(float *out, uint32_t outLen, uint32_t inLen) {(void)out;(void)outLen;(void)inLen;};

	public:
		Player();
		virtual ~Player();
		void start();
		void stop();
		void setPitch(float pitch);
		void setGain(float gain);
		void setPan(float pan);
		void setLoop(bool loop);
		inline bool isPlaying() {return this->active;};

	};

}

#endif
