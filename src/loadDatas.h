#ifndef LIBAUDIO_LOAD_DATAS_H
# define LIBAUDIO_LOAD_DATAS_H

# include <cstdint>
# include <string>

namespace libaudio
{

	int32_t getPCMSize(std::string file);
	int32_t loadDatas(std::string file, int16_t **datas, int *rate, int *channelsCount);

}

#endif
