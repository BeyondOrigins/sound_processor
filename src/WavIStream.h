#ifndef SOUND_PROCESSOR_WAVISTREAM_H
#define SOUND_PROCESSOR_WAVISTREAM_H
#include <istream>


class WavIStream
{
public:
    WavIStream() = default;
    WavIStream(const WavIStream &) = delete;
    WavIStream &operator=(const WavIStream &) = delete;
    WavIStream(WavIStream &&) = default;
    WavIStream &operator=(WavIStream &&) = default;
    WavIStream(std::istream &stream) : _stream(&stream) {}
    ~WavIStream() = default;
private:
    std::istream* _stream;
};


#endif //SOUND_PROCESSOR_WAVISTREAM_H