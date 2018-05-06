#include "Stream.h"
#include "CachedPlayer.h"
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <cmath>

using libaudio::Stream;
using libaudio::CachedPlayer;

int main()
{
	srand(time(NULL));
	PaError error = Pa_Initialize();
	if (error)
	{
		std::cerr << "Failed to initialize portaudio" << std::endl;
		return (EXIT_FAILURE);
	}
	Stream *stream = new Stream();
	float *datas = new float[44100 * 2];
	for (uint32_t i = 0; i < 44100 * 2; i += 2)
	{
		datas[i] = rand() / (float)RAND_MAX;
		datas[i + 1] = datas[i];
	}
	CachedPlayer *player = new CachedPlayer(datas, 44100 * 2, 44100);
	stream->addPlayer(player);
	player->start();
	stream->start();
	for (uint32_t i = 0; i < 1000; ++i)
	{
		stream->setPitch(i / 1000.);
		usleep(10000);
	}
}
