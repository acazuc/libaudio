#include "CachedPlayer.h"

namespace libaudio
{

	CachedPlayer::CachedPlayer(float *data, uint32_t length, uint32_t rate)
	: position(0)
	, length(length)
	, data(data)
	{
		this->rate = rate;
	}

	void CachedPlayer::getData(float *out, unsigned long frameCount)
	{
		if (!this->length)
			return;
		for (uint32_t i = 0; i < frameCount; ++i)
		{
			out[i] = this->data[this->position];
			if (++this->position >= this->length)
				this->position = 0;
		}
	}

}
