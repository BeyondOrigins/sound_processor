#ifndef SOUND_PROCESSOR_WAVISTREAM_H
#define SOUND_PROCESSOR_WAVISTREAM_H
#include "WavBaseStream.h"
#include <cstring>

class Waveform;

// class for reading data from WAV file
class WavIStream: public WavBaseStream
{
public:
    explicit WavIStream(const std::string& fileName)
    {
        WavIStream::open(fileName);
    }

public:
    void open(const std::string& fileName) override;
    friend WavIStream& operator>>(WavIStream& stream, Waveform& data);
};

#endif  // SOUND_PROCESSOR_WAVISTREAM_H