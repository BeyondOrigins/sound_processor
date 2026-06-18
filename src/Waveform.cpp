#include "Waveform.h"
#include "WavIStream.h"
#include "WavOStream.h"

WavIStream& operator>>(WavIStream& stream, Waveform& data)
{
    if(!stream.isOpen())
        throw std::ios_base::failure("Stream is closed");
    data._info = stream._info;
    const size_t COUNT = stream._info.data.size / sizeof(int16_t);
    data._data.resize(COUNT);
    if(COUNT > 0)
        stream._stream.read(
            reinterpret_cast<char*>(data._data.data()),
            static_cast<std::streamsize>(COUNT * sizeof(int16_t)));
    return stream;
}

WavOStream& operator<<(WavOStream& stream, const Waveform& data)
{
    if(!stream.isOpen())
        throw std::ios_base::failure("Stream is closed");
    stream._stream.write(reinterpret_cast<const char*>(&data._info),
                         sizeof(WavInfo));
    if(!data._data.empty())
        stream._stream.write(
            reinterpret_cast<const char*>(data._data.data()),
            static_cast<std::streamsize>(data._data.size() * sizeof(int16_t)));
    return stream;
}

void Waveform::syncHeader()
{
    constexpr uint16_t CHANNELS = 1;
    constexpr uint32_t RATE = 44100;
    constexpr uint16_t BITS = 16;
    constexpr uint16_t BLOCK = CHANNELS * (BITS / 8);
    const auto DATA_SIZE =
        static_cast<uint32_t>(_data.size() * sizeof(int16_t));

    std::memcpy(&_info.riff.chunkId, "RIFF", 4);
    _info.riff.size = DATA_SIZE + 36;
    std::memcpy(&_info.riff.waveId, "WAVE", 4);

    std::memcpy(&_info.fmt.chunkId, "fmt ", 4);
    _info.fmt.size = 16;
    _info.fmt.wFormatTag = 1;
    _info.fmt.wChannels = CHANNELS;
    _info.fmt.dwSamplesPerSec = RATE;
    _info.fmt.dwAvgBytesPerSec = RATE * BLOCK;
    _info.fmt.wBlockAlign = BLOCK;
    _info.fmt.wBitsPerSamp = BITS;

    std::memcpy(&_info.data.chunkId, "data", 4);
    _info.data.size = DATA_SIZE;
}
