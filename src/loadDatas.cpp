#include "loadDatas.h"
#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>
#include <cstring>
#include <vector>
#include <cstdio>

namespace libaudio
{

	int loadDatas(std::string filename, int16_t **datas, size_t *len, int *rate, int *channelsCount)
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
		int16_t *buffer = NULL;
		size_t bufLen = 0;
		int16_t *buf2;
		int16_t *tmp = new int16_t[4096 / 2];
		long ret;
		int osef;
		while ((ret = ov_read(&vorbisFile, reinterpret_cast<char*>(tmp), 4096, 0, 2, 1, &osef)) > 0)
		{
			buf2 = new int16_t[bufLen + ret];
			if (buffer)
			{
				std::memcpy(buf2, buffer, bufLen);
				delete[] (buffer);
			}
			std::memcpy(buf2 + bufLen, tmp, ret);
			buffer = buf2;
			bufLen += ret;
		}
		*len = bufLen;
		delete[] (tmp);
		*datas = buffer;
		ov_clear(&vorbisFile);
		std::fclose(file);
		if (ret == -1)
		{
			if (buffer)
				delete[] (buffer);
			return (-1);
		}
		return (0);
	}

}
