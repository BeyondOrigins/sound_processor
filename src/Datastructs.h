#ifndef SOUND_PROCESSOR_DATASTRUCTS_H
#define SOUND_PROCESSOR_DATASTRUCTS_H
#include <cstdint>

struct __attribute__((packed)) RiffHeader
{
    uint32_t chunkId;  // "RIFF"
    uint32_t size;     // chunk data size
    uint32_t waveId;   // "WAVE"
};

struct __attribute__((packed)) FmtHeader
{
    uint32_t chunkId;           // "fmt "
    uint32_t size;              // chunk data size
    uint16_t wFormatTag;        // compression code
    uint16_t wChannels;         // number of channels
    uint32_t dwSamplesPerSec;   // sample rate
    uint32_t dwAvgBytesPerSec;  // average bytes per second
    uint16_t wBlockAlign;       // block align
    uint16_t wBitsPerSamp;      // significant bits per sample
};

struct __attribute__((packed)) DataHeader
{
    uint32_t chunkId;  // "data"
    uint32_t size;     // chunk data size
};

struct __attribute__((packed)) WavInfo
{
    RiffHeader riff;  // RIFF header
    FmtHeader fmt;    // FMT  header
    DataHeader data;  // DATA header
};

#endif  // SOUND_PROCESSOR_DATASTRUCTS_H