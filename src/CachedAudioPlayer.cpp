#include "CachedAudioPlayer.h"
#include "Exception.h"
#include <cstring>

namespace libaudio
{

	static int CachedAudioPlayerCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *paTimeInfo, PaStreamCallbackFlags statusFlags, void *userData)
	{
		(void)input;
		(void)paTimeInfo;
		(void)statusFlags;
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
			if (audioPlayer->getGain() == 0)
				std::memset(out, 0, available);
			else
			{
				std::memmove(out, reinterpret_cast<char*>(audioPlayer->datas) + audioPlayer->pos, available);
				if (audioPlayer->getGain() != 1)
				{
					for (size_t i = 0; i < available / 2; ++i)
						reinterpret_cast<int16_t*>(out)[i] *= audioPlayer->getGain();
				}
			}
			frameCount -= available;
			audioPlayer->pos += available;
			out += available;
			readed = 1;
		}
		return (paContinue);
	}

	CachedAudioPlayer::CachedAudioPlayer(int16_t *datas, size_t len, int rate, int channelsCount)
	: datas(datas)
	, len(len)
	, pos(0)
	{
		this->outputParameters.device = Pa_GetDefaultOutputDevice();
		const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(this->outputParameters.device);
		this->outputParameters.channelCount = channelsCount;
		this->outputParameters.sampleFormat = paInt16;
		this->outputParameters.suggestedLatency = deviceInfo->defaultHighOutputLatency;
		this->outputParameters.hostApiSpecificStreamInfo = NULL;
		PaError error = Pa_OpenStream(&this->stream, NULL, &outputParameters, rate, rate / 20, paNoFlag, CachedAudioPlayerCallback, this);
		if (error)
			throw std::exception();
	}

	void CachedAudioPlayer::stop()
	{
		AudioPlayer::stop();
		this->pos = 0;
	}

}
