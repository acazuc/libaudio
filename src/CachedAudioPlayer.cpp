#include "CachedAudioPlayer.h"
#include "Exception.h"
#include <cstring>

namespace libaudio
{

	static int CachedAudioPlayerCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *paTimeInfo, PaStreamCallbackFlags statusFlags, void *userData)
	{
		CachedAudioPlayer *audioPlayer = reinterpret_cast<CachedAudioPlayer*>(userData);
		char *out = reinterpret_cast<char*>(output);
		int readed = 0;
		frameCount *= audioPlayer->outputParameters.channelCount * 2;
		while (frameCount > 0)
		{
			size_t available = audioPlayer->len - audioPlayer->pos;
			if (available == 0)
			{
				if (audioPlayer->isLoop())
				{
					available = audioPlayer->len;
					audioPlayer->pos = 0;
				}
				else
				{
					if (readed)
					{
						std::memset(out, 0, frameCount);
						return (paContinue);
					}
					return (paAbort);
				}
			}
			if (available > frameCount)
				available = frameCount;
			std::memcpy(out, audioPlayer->datas + audioPlayer->pos, available);
			for (long i = 0; i < available; ++i)
				reinterpret_cast<int16_t*>(out)[i] *= audioPlayer->getGain();
			frameCount -= available;
			out += available;
			readed = 1;
		}
		return (paContinue);
	}

	CachedAudioPlayer::CachedAudioPlayer(char *datas, size_t len, int rate, int channelsCount)
	: datas(reinterpret_cast<int16_t*>(datas))
	, len(len)
	, pos(0)
	{
		this->outputParameters.device = Pa_GetDefaultOutputDevice();
		this->outputParameters.channelCount = channelsCount;
		this->outputParameters.sampleFormat = paInt16;
		this->outputParameters.suggestedLatency = 0.2;
		this->outputParameters.hostApiSpecificStreamInfo = 0;
		PaError error = Pa_OpenStream(&this->stream, 0, &outputParameters, rate, rate / 20, paNoFlag, CachedAudioPlayerCallback, this);
		if (error)
			throw std::exception();
	}

	void CachedAudioPlayer::stop()
	{
		AudioPlayer::stop();
		this->pos = 0;
	}

}
