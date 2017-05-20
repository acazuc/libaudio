#include "BufferedAudioPlayer.h"
#include "Exception.h"
#include <cstring>

namespace libaudio
{

	static int BufferedAudioPlayerCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *paTimeInfo, PaStreamCallbackFlags statusFlags, void *userData)
	{
		BufferedAudioPlayer *audioPlayer = reinterpret_cast<BufferedAudioPlayer*>(userData);
		char *out = reinterpret_cast<char*>(output);
		frameCount *= audioPlayer->outputParameters.channelCount * 2;
		int readed = 0;
		int osef;
		while (frameCount > 0)
		{
			long ret = ov_read(&audioPlayer->vorbisFile, out, frameCount, 0, 2, 1, &osef);
			if (ret == 0)
			{
				if (audioPlayer->isLoop())
				{
					if (ov_raw_seek(&audioPlayer->vorbisFile, 0))
						return (paAbort);
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
			else if (ret < 0)
				return (paAbort);
			else
			{
				if (audioPlayer->getGain() == 0)
					std::memset(out, 0, ret * 2);
				else if (audioPlayer->getGain() != 1)
				{
					for (long i = 0; i < ret / 2; ++i)
						reinterpret_cast<int16_t*>(out)[i] *= audioPlayer->getGain();
				}
				frameCount -= ret;
				out += ret;
				readed = 1;
			}
		}
		return (paContinue);
	}

	BufferedAudioPlayer::BufferedAudioPlayer(std::string file)
	{
		AudioPlayer();
		this->file = std::fopen(file.c_str(), "rb");
		if (!this->file)
			throw Exception("Failed to open file");
		if (ov_open_callbacks(this->file, &this->vorbisFile, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0)
		{
			std::fclose(this->file);
			throw Exception("Failed to open ogg file");
		}
		if (!(this->vorbisInfos = ov_info(&this->vorbisFile, -1)))
		{
			ov_clear(&this->vorbisFile);
			std::fclose(this->file);
			throw Exception("Failed to get ogg file infos");
		}
		outputParameters.device = Pa_GetDefaultOutputDevice();
		outputParameters.channelCount = this->vorbisInfos->channels;
		outputParameters.sampleFormat = paInt16;
		outputParameters.suggestedLatency = 0.2;
		outputParameters.hostApiSpecificStreamInfo = 0;
		PaError error = Pa_OpenStream(&this->stream, 0, &outputParameters, this->vorbisInfos->rate, this->vorbisInfos->rate / 20, paNoFlag, BufferedAudioPlayerCallback, this);
		if (error)
		{
			ov_clear(&this->vorbisFile);
			std::fclose(this->file);
			throw Exception("Failed to create portaudio stream");
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
