#include "WavIStream.h"

void WavIStream::open(const std::string& fileName)
{
    _stream.exceptions(std::ios::failbit | std::ios::badbit);
    _stream.open(fileName, std::ios::binary | std::ios::in);
    _stream.read(reinterpret_cast<char*>(&_info.riff), sizeof(RiffHeader));
    _stream.read(reinterpret_cast<char*>(&_info.fmt), sizeof(FmtHeader));
    _stream.read(reinterpret_cast<char*>(&_info.data), sizeof(DataHeader));
    if(std::memcmp(&_info.riff.chunkId, "RIFF", 4) != 0 ||
       std::memcmp(&_info.riff.waveId, "WAVE", 4) != 0 ||
       std::memcmp(&_info.fmt.chunkId, "fmt ", 4) != 0 ||
       std::memcmp(&_info.data.chunkId, "data", 4) != 0)
        throw std::runtime_error("Not a valid WAV file");
    if(_info.fmt.wFormatTag != 1 || _info.fmt.wChannels != 1 ||
       _info.fmt.dwSamplesPerSec != 44100 || _info.fmt.wBitsPerSamp != 16)
        throw std::runtime_error(
            "Unsupported WAV format (need PCM mono 44100/16)");
}