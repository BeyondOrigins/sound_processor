#ifndef SOUND_PROCESSOR_WAVEFORM_H
#define SOUND_PROCESSOR_WAVEFORM_H
#include <vector>

class Waveform
{
    struct __attribute__((packed)) DataInfo
    {
        uint16_t wFormatTag = 1;
        uint16_t wChannels = 1;
        uint32_t dwSamplesPerSec = 44100;
        uint32_t dwAvgBytesPerSec;
        uint16_t wBlockAlign = 2;
        uint16_t wBitsPerSample = 16;
        uint32_t dwDataSize;
    };
public:
    Waveform() = default;
public:
    const std::vector<int16_t>& getData() const { return _data; }

private:
    std::vector<int16_t> _data;
    DataInfo _dataInfo;
};

#endif  // SOUND_PROCESSOR_WAVEFORM_H