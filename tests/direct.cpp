#include "Stream.h"
#include "loadDatas.h"
#include "DirectPlayer.h"
#include <cstdlib>
#include <unistd.h>
#include <iostream>

using libaudio::Stream;
using libaudio::DirectPlayer;

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
	DirectPlayer *player = new CachedPlayer("./test.ogg");
	player->setLoop(true);
	stream->addPlayer(player);
	player->start();
	stream->start();
	sleep(5);
}
