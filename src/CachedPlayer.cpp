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

	void CachedPlayer::getData(float *out, uint32_t outLen, uint32_t inLen)
	{
		if (!this->length)
			return;
		float ratio = inLen / static_cast<float>(outLen);
		for (uint32_t i = 0; i < outLen; i += 2)
		{
			uint32_t pos = (uint32_t)(this->position + i * ratio) % this->length;
			out[i] = this->data[pos];
			out[i + 1] = this->data[pos + 1];
		}
		this->position += inLen;
		this->position %= this->length;
	}

}
