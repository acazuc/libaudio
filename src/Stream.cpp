#include "Stream.h"
#include "Exception.h"

namespace libaudio
{

	int Stream::callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *paTimeInfo, PaStreamCallbackFlags statusFlags, void *userData)
	{
		(void)statusFlags;
		(void)paTimeInfo;
		(void)input;
		Stream *stream = reinterpret_cast<Stream*>(userData);
		float *out = reinterpret_cast<float*>(output);
		if (!frameCount)
			return (paContinue);
		std::vector<float> tmp(frameCount * 2);
		for (uint32_t i = 0; i < frameCount * 2; ++i)
			out[i] = 0;
		for (uint32_t i = 0; i < stream->players.size(); ++i)
		{
			stream->players[i]->callback(tmp.data(), frameCount, stream->rate);
			for (uint32_t i = 0; i < frameCount * 2; ++i)
				out[i] += tmp[i];
		}
		if (stream->gain == 0)
		{
			for (uint32_t i = 0; i < frameCount * 2; ++i)
				out[i] = 0;
			return (paContinue);
		}
		if (stream->gain != 1)
		{
			for (uint32_t i = 0; i < frameCount * 2; ++i)
				out[i] *= stream->gain;
		}
		if (stream->pan != .5)
		{
			for (uint32_t i = 0; i < frameCount * 2; ++i)
			{
				if (i & 1)
					out[i] *= std::max(0.f, stream->pan * 2);
				else
					out[i] *= std::max(0.f, (1 - stream->pan) * 2);
			}
		}
		return (paContinue);
	}

	Stream::Stream()
	: pitch(1)
	, gain(1)
	, pan(.5)
	{
		this->outputParameters.device = Pa_GetDefaultOutputDevice();
		const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(this->outputParameters.device);
		if (!deviceInfo)
			throw Exception("Invalid default device");
		this->outputParameters.channelCount = 2;
		this->outputParameters.sampleFormat = paFloat32;
		this->outputParameters.suggestedLatency = deviceInfo->defaultHighOutputLatency;
		this->outputParameters.hostApiSpecificStreamInfo = nullptr;
		this->rate = deviceInfo->defaultSampleRate;
		PaError error = Pa_OpenStream(&this->stream, nullptr, &outputParameters, this->rate, paFramesPerBufferUnspecified, paNoFlag, &Stream::callback, this);
		if (error)
			throw Exception("Pa_OpenStream() error: " + std::string(Pa_GetErrorText(error)));
	}

	Stream::~Stream()
	{
		Pa_CloseStream(this->stream);
	}

	void Stream::start()
	{
		PaError error = Pa_StartStream(this->stream);
		if (error)
			throw Exception("Pa_StartStream() error: " + std::string(Pa_GetErrorText(error)));
	}

	void Stream::stop()
	{
		PaError error = Pa_AbortStream(this->stream);
		if (error)
			throw Exception("Pa_AbortStream() error: " + std::string(Pa_GetErrorText(error)));
	}

	void Stream::setPitch(float pitch)
	{
		this->pitch = std::max(0.f, pitch);
	}

	void Stream::setGain(float gain)
	{
		this->gain = std::max(0.f, std::min(1.f, gain));
	}

	void Stream::setPan(float pan)
	{
		this->pan = std::max(0.f, std::min(1.f, pan));
	}

	void Stream::addPlayer(Player *player)
	{
		this->players.push_back(player);
	}

	void Stream::removePlayer(Player *player)
	{
		for (uint32_t i = 0; i < this->players.size(); ++i)
		{
			if (this->players[i] != player)
				continue;
			this->players.erase(this->players.begin() + i);
			return;
		}
	}

}
