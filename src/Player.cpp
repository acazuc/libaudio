#include "Player.h"
#include <algorithm>
#include <vector>

namespace libaudio
{

	Player::Player()
	: pitch(1)
	, gain(1)
	, pan(.5)
	, active(false)
	{
		//Empty
	}

	Player::~Player()
	{
		//Empty
	}

	void Player::callback(float *out, unsigned long frameCount, uint32_t rate)
	{
		if (!frameCount)
			return;
		getData(out, frameCount * 2);
		if (this->gain == 0)
		{
			for (uint32_t i = 0; i < frameCount * 2; ++i)
				out[i] = 0;
			return;
		}
		if (this->gain != 1)
		{
			for (uint32_t i = 0; i < frameCount * 2; ++i)
				out[i] *= this->gain;
		}
		if (this->pan != .5)
		{
			for (uint32_t i = 0; i < frameCount * 2; ++i)
			{
				if (i & 1)
					out[i] *= std::max(0.f, this->pan * 2);
				else
					out[i] *= std::max(0.f, (1 - this->pan) * 2);
			}
		}
	}

	void Player::start()
	{
		this->active = true;
	}

	void Player::stop()
	{
		this->active = false;
	}

	void Player::setPitch(float pitch)
	{
		this->pitch = pitch;
	}

	void Player::setGain(float gain)
	{
		this->gain = std::max(0.f, std::min(1.f, gain));
	}

	void Player::setPan(float pan)
	{
		this->pan = std::max(0.f, std::min(1.f, pan));
	}

}
