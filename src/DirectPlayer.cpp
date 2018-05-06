#include "DirectPlayer.h"
#include "Exception.h"

namespace libaudio
{

	DirectPlayer::DirectPlayer(std::string file)
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
		this->rate = this->vorbisInfos->rate;
	}

	void DirectPlayer::getData(float *out, uint32_t outLen, uint32_t inLen)
	{
		//
	}

}
