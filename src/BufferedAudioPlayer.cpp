#include "BufferedAudioPlayer.h"
#include "Exception.h"
#include <math.h>
#include <cstring>

namespace libaudio
{

	int BufferedAudioPlayer::callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *paTimeInfo, PaStreamCallbackFlags statusFlags, void *userData)
	{
		(void)input;
		(void)paTimeInfo;
		(void)statusFlags;
		BufferedAudioPlayer *audioPlayer = reinterpret_cast<BufferedAudioPlayer*>(userData);
		float *out = reinterpret_cast<float*>(output);
		bool readed = false;
		bool hasLooped = false;
		while (frameCount > 0)
		{
			float **datas;
			long ret = ov_read_float(&audioPlayer->vorbisFile, &datas, frameCount, NULL);
			if (ret == 0)
			{
				if (audioPlayer->isLoop())
				{
					if (hasLooped)
						return (paAbort);
					if (ov_raw_seek(&audioPlayer->vorbisFile, 0))
						return (paAbort);
					hasLooped = true;
				}
				else
				{
					if (readed)
					{
						std::memset(out, 0, frameCount * sizeof(*out));
						return (paContinue);
					}
					return (paAbort);
				}
			}
			else if (ret < 0)
			{
				return (paAbort);
			}
			else
			{
				hasLooped = false;
				if (audioPlayer->getGain() <= 0)
				{
					std::memset(out, 0, ret * sizeof(*out));
				}
				else if (audioPlayer->getGain() >= 1)
				{
					for (long i = 0; i < ret; ++i)
					{
						for (long j = 0; j < audioPlayer->outputParameters.channelCount; ++j)
							*out++ = datas[j][i];
					}
				}
				else if (audioPlayer->getGain() != 1)
				{
					for (long i = 0; i < ret; ++i)
					{
						for (long j = 0; j < audioPlayer->outputParameters.channelCount; ++j)
							*out++ = datas[j][i] * audioPlayer->getGain();
					}
				}
				frameCount -= ret;
				readed = true;
			}
		}
		return (paContinue);
	}

	BufferedAudioPlayer::BufferedAudioPlayer(std::string file)
	{
		this->file = std::fopen(file.c_str(), "rb");
		if (!this->file)
			throw Exception("fopen() error");
		int ov_error = ov_open_callbacks(this->file, &this->vorbisFile, NULL, 0, OV_CALLBACKS_NOCLOSE);
		if (ov_error)
		{
			std::fclose(this->file);
			throw Exception("ov_open_callbacks() error: " + std::to_string(ov_error));
		}
		if (!(this->vorbisInfos = ov_info(&this->vorbisFile, -1)))
		{
			ov_clear(&this->vorbisFile);
			std::fclose(this->file);
			throw Exception("ov_info() error");
		}
		this->outputParameters.device = Pa_GetDefaultOutputDevice();
		const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(this->outputParameters.device);
		this->outputParameters.channelCount = this->vorbisInfos->channels;
		this->outputParameters.sampleFormat = paFloat32;
		this->outputParameters.suggestedLatency = deviceInfo->defaultHighOutputLatency;
		this->outputParameters.hostApiSpecificStreamInfo = NULL;
		PaError Pa_error = Pa_OpenStream(&this->stream, NULL, &this->outputParameters, this->vorbisInfos->rate, paFramesPerBufferUnspecified, paNoFlag, &BufferedAudioPlayer::callback, this);
		if (Pa_error)
		{
			ov_clear(&this->vorbisFile);
			std::fclose(this->file);
			throw Exception("Pa_OpenStream() error: " + std::string(Pa_GetErrorText(Pa_error)));
		}
	}

	BufferedAudioPlayer::~BufferedAudioPlayer()
	{
		ov_clear(&this->vorbisFile);
		std::fclose(this->file);
	}

	void BufferedAudioPlayer::stop()
	{
		AudioPlayer::stop();
		if (ov_raw_seek(&this->vorbisFile, 0))
			throw Exception("Failed to seek file");
	}

}
