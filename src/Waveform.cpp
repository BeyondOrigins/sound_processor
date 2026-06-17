#include "Waveform.h"
#include "WavIStream.h"
#include "WavOStream.h"

WavIStream& operator>>(WavIStream& stream, Waveform& data)
{
    if (!stream.isOpen())
        throw std::ios_base::failure("Stream is closed");
    data._info = stream._info;
    const size_t COUNT = stream._info.data.size / sizeof(int16_t);
    data._data.resize(COUNT);
    if (COUNT > 0)
        stream._stream.read(reinterpret_cast<char*>(data._data.data()),
                            static_cast<std::streamsize>(COUNT * sizeof(int16_t)));
    return stream;
}

WavOStream& operator<<(WavOStream& stream, const Waveform& data)
{
    if (!stream.isOpen())
        throw std::ios_base::failure("Stream is closed");
    stream._stream.write(reinterpret_cast<const char*>(&data._info),
                         sizeof(WavInfo));
    stream._stream.write(reinterpret_cast<const char*>(&data._data),
                         data._info.data.size);
    return stream;
}

void Waveform::syncHeader()
{
    size_t dataSize = _data.size()*sizeof(int16_t);
    _info.data.size = dataSize;
    _info.riff.size = dataSize + 36;
}
