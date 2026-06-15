#ifndef SOUND_PROCESSOR_WAVEFORM_H
#define SOUND_PROCESSOR_WAVEFORM_H
#include <vector>
#include "Datastructs.h"

class WavIStream;
class WavOStream;

class Waveform
{

public:
    Waveform() = default;
    Waveform(const Waveform& other)
        : _info(other._info), _data(other._data)
    {
    }
    Waveform& operator=(const Waveform& other)
    {
        if(&other != this)
        {
            _data = other._data;
            _info = other._info;
        }
        return *this;
    }
    Waveform(Waveform&& other) noexcept
        : _info(std::move(other._info)), _data(std::move(other._data))
    {
    }
    Waveform& operator=(Waveform&& other) noexcept
    {
        if(&other != this)
        {
            _info = std::move(other._info);
            _data = std::move(other._data);
        }
        return *this;
    }
    Waveform(const WavInfo& dataInfo, std::vector<int16_t>& data)
        : _info(std::move(dataInfo)), _data(std::move(data))
    {
    }
    ~Waveform() = default;

    friend WavIStream& operator>>(WavIStream& stream, Waveform& data);
    friend WavOStream& operator<<(WavOStream& stream, const Waveform& data);

public:
    const std::vector<int16_t>& getData() const { return _data; }
    const WavInfo& getDataInfo() const { return _info; }

    // TODO: develop methods for interacting with data
    // probably need to create a member defining whether the object stores data

private:
    std::vector<int16_t> _data;
    WavInfo _info;
};

#endif  // SOUND_PROCESSOR_WAVEFORM_H