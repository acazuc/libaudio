#ifndef LIBAUDIO_LOAD_DATAS_H
# define LIBAUDIO_LOAD_DATAS_H

# include <cstdint>
# include <string>

namespace libaudio
{

	int loadDatas(std::string file, int16_t **datas, size_t *len, int *rate, int *channelsCount);

}

#endif
