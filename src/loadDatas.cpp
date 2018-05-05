#include "loadDatas.h"
#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>
#include <cstring>
#include <vector>
#include <cstdio>

namespace libaudio
{

	int32_t getPCMSize(std::string filename)
	{
		OggVorbis_File vorbisFile;
		FILE *file = std::fopen(filename.c_str(), "rb");
		if (!file)
			return (-1);
		if (ov_open_callbacks(file, &vorbisFile, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0)
		{
			std::fclose(file);
			return (-1);
		}
		ogg_int64_t len = ov_pcm_total(&vorbisFile, -1);
		std::fclose(file);
		if (len < 0)
			return (-1);
		return (len);
	}

	int32_t loadDatas(std::string filename, int16_t **datas, int *rate, int *channelsCount)
	{
		OggVorbis_File vorbisFile;
		vorbis_info *vorbisInfos;
		FILE *file = std::fopen(filename.c_str(), "rb");
		if (!file)
			return (-1);
		if (ov_open_callbacks(file, &vorbisFile, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0)
		{
			std::fclose(file);
			return (-1);
		}
		if (!(vorbisInfos = ov_info(&vorbisFile, -1)))
		{
			ov_clear(&vorbisFile);
			std::fclose(file);
			return (-1);
		}
		*rate = vorbisInfos->rate;
		*channelsCount = vorbisInfos->channels;
		char *buffer = NULL;
		size_t bufLen = 0;
		char *buf2;
		char *tmp = new char[4096];
		long ret;
		int osef;
		while ((ret = ov_read(&vorbisFile, tmp, 4096, 0, 2, 1, &osef)) > 0)
		{
			buf2 = new char[bufLen + ret];
			if (buffer)
			{
				std::memcpy(buf2, buffer, bufLen);
				delete[] (buffer);
			}
			std::memcpy(buf2 + bufLen, tmp, ret);
			buffer = buf2;
			bufLen += ret;
		}
		delete[] (tmp);
		*datas = reinterpret_cast<int16_t*>(buffer);
		ov_clear(&vorbisFile);
		std::fclose(file);
		if (ret < 0)
		{
			delete[] (buffer);
			return (-1);
		}
		return (bufLen);
	}

}
