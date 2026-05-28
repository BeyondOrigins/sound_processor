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
    Waveform(const Waveform& other) : _dataInfo(other._dataInfo), _data(other._data) {}
    Waveform& operator=(const Waveform& other)
    {
        if (&other != this)
        {
            _data = other._data;
            _dataInfo = other._dataInfo;
        }
        return *this;
    }
    Waveform(Waveform&& other) noexcept : _dataInfo(std::move(other._dataInfo)), _data(std::move(other._data)) {}
    Waveform& operator=(Waveform&& other) noexcept
    {
        if (&other != this)
        {
            _dataInfo = std::move(other._dataInfo);
            _data = std::move(other._data);
        }
        return *this;
    }
    ~Waveform() = default;
public:
    const std::vector<int16_t>& getData() const { return _data; }
    const DataInfo& getDataInfo() const { return _dataInfo; }
private:
    std::vector<int16_t> _data;
    DataInfo _dataInfo;
};

#endif  // SOUND_PROCESSOR_WAVEFORM_H