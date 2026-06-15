#include "WavBaseStream.h"

void WavBaseStream::open(const std::string& fileName)
{
    _stream.open(fileName, std::ios::in | std::ios::binary | std::ios::out);
}

void WavBaseStream::close()
{
    _stream.close();
}
