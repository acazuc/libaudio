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
			return paContinue;
		std::vector<float> tmp(frameCount * 2);
		std::fill(out, out + frameCount * 2, 0);
		for (size_t i = 0; i < stream->players.size(); ++i)
		{
			if (!stream->players[i]->callback(tmp.data(), frameCount, stream->sampling / stream->pitch))
				continue;
			for (size_t i = 0; i < frameCount * 2; ++i)
				out[i] += tmp[i];
		}
		for (size_t i = 0; i < stream->filters.size(); ++i)
			stream->filters[i]->filter(out, frameCount);
		if (stream->gain == 0)
		{
			std::fill(out, out + frameCount * 2, 0);
			return paContinue;
		}
		if (stream->gain != 1)
		{
			for (size_t i = 0; i < frameCount * 2; ++i)
				out[i] *= stream->gain;
		}
		if (stream->pan != .5)
		{
			for (size_t i = 0; i < frameCount; ++i)
			{
				float pan2 = stream->pan * 2;
				out[i] *= std::max(0.f, 1 - pan2);
				out[i + 1] *= std::max(0.f, pan2);
			}
		}
		return paContinue;
	}

	Stream::Stream(uint8_t channels, uint32_t sampling)
	: sampling(sampling)
	, channels(channels)
	, pitch(1)
	, gain(1)
	, pan(.5)
	{
		this->outputParameters.device = Pa_GetDefaultOutputDevice();
		const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(this->outputParameters.device);
		if (!deviceInfo)
			throw Exception("Pa_GetDeviceInfo() failed");
		this->outputParameters.channelCount = this->channels;
		this->outputParameters.sampleFormat = paFloat32;
		this->outputParameters.suggestedLatency = deviceInfo->defaultHighOutputLatency;
		this->outputParameters.hostApiSpecificStreamInfo = nullptr;
		PaError error = Pa_OpenStream(&this->stream, nullptr, &outputParameters, this->sampling, paFramesPerBufferUnspecified, paNoFlag, &Stream::callback, this);
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

	void Stream::addPlayer(Player *player)
	{
		this->players.push_back(player);
	}

	void Stream::removePlayer(Player *player)
	{
		for (size_t i = 0; i < this->players.size(); ++i)
		{
			if (this->players[i] != player)
				continue;
			this->players.erase(this->players.begin() + i);
			return;
		}
	}

	void Stream::addFilter(Filter *filter)
	{
		for (size_t i = 0; i < this->filters.size(); ++i)
		{
			if (this->filters[i] == filter)
				return;
		}
		this->filters.push_back(filter);
	}

	void Stream::removeFilter(Filter *filter)
	{
		for (size_t i = 0; i < this->filters.size(); ++i)
		{
			if (this->filters[i] != filter)
				continue;
			this->filters.erase(this->filters.begin() + i);
			return;
		}
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

}
