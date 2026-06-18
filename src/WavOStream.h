#ifndef SOUND_PROCESSOR_WAVOSTREAM_H
#define SOUND_PROCESSOR_WAVOSTREAM_H
#include "WavBaseStream.h"

class Waveform;

// class for writing data to WAV file
class WavOStream: public WavBaseStream
{
public:
    explicit WavOStream(const std::string& fileName)
    {
        _stream.exceptions(std::ios::failbit | std::ios::badbit);
        _stream.open(fileName, std::ios::binary | std::ios::out);
    }

public:
    void open(const std::string& fileName) override;
    friend WavOStream& operator<<(WavOStream& stream, const Waveform& data);
};

#endif  // SOUND_PROCESSOR_WAVOSTREAM_H