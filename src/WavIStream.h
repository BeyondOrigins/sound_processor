#ifndef SOUND_PROCESSOR_WAVISTREAM_H
#define SOUND_PROCESSOR_WAVISTREAM_H
#include "WavBaseStream.h"

class Waveform;

// class for reading data from WAV file
class WavIStream: public WavBaseStream
{
public:
    explicit WavIStream(const std::string& fileName)
    {
        _stream.exceptions(std::ios::failbit | std::ios::badbit);
        _stream.open(fileName, std::ios::binary | std::ios::in);
        _stream.read(reinterpret_cast<char*>(&_info.riff), sizeof(RiffHeader));
        _stream.read(reinterpret_cast<char*>(&_info.fmt), sizeof(FmtHeader));
        _stream.read(reinterpret_cast<char*>(&_info.data), sizeof(DataHeader));
    }

public:
    friend WavIStream& operator>>(WavIStream& stream, Waveform& data);
};

#endif  // SOUND_PROCESSOR_WAVISTREAM_H