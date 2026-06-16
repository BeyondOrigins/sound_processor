#include "WavIStream.h"

void WavIStream::open(const std::string& filName)
{
    _stream.open(filName, std::ios::in | std::ios::binary);
}