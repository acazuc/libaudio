#ifndef LIBAUDIO_CACHED_PLAYER_H
# define LIBAUDIO_CACHED_PLAYER_H

# include "Player.h"

namespace libaudio
{

	class CachedPlayer : public Player
	{

	private:
		uint32_t position;
		uint32_t length;
		float *data;
		void getData(float *out, unsigned long frameCount);

	public:
		CachedPlayer(float *data, uint32_t length, uint32_t rate);

	};

}

#endif
