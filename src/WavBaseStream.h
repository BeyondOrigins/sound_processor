#ifndef SOUND_PROCESSOR_WAVBASESTREAM_H
#define SOUND_PROCESSOR_WAVBASESTREAM_H
#include "Datastructs.h"
#include <fstream>

class WavBaseStream
{
public:
    WavBaseStream(): _info()
    {
        _stream.exceptions(std::ios::failbit | std::ios::badbit);
    }
    WavBaseStream(const WavBaseStream&) = delete;
    WavBaseStream& operator=(const WavBaseStream&) = delete;
    WavBaseStream(WavBaseStream&&) = delete;
    WavBaseStream& operator=(WavBaseStream&&) = delete;
protected:
    ~WavBaseStream() = default;

public:
    bool operator!() const { return !_stream; }
    bool isOpen() const { return _stream.is_open(); }
    void close();
    virtual void open(const std::string& fileName) = 0;

protected:
    WavInfo _info;
    std::fstream _stream;
};

#endif  // SOUND_PROCESSOR_WAVBASESTREAM_H