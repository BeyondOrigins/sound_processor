#include "WavOStream.h"

void WavOStream::open(const std::string& fileName)
{
    _stream.open(fileName, std::ios::out | std::ios::binary);
}