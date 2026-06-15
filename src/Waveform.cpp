#include "Waveform.h"
#include "WavIStream.h"
#include "WavOStream.h"

WavIStream& operator>>(WavIStream& stream, Waveform& data)
{
    if (!stream.isOpen())
        throw std::ios_base::failure("Stream is closed");
    stream._stream.read(reinterpret_cast<char*>(&data._data),
                        stream._info.data.size);
    data._info = stream._info;
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