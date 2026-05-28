#ifndef SOUND_PROCESSOR_DATASTRUCTS_H
#define SOUND_PROCESSOR_DATASTRUCTS_H
#include <cstdint>

struct __attribute__((packed)) RiffHeader
{
    uint32_t chunkId;
    uint32_t size;
    uint32_t waveId;
};

struct __attribute__((packed)) FmtHeader
{
    uint32_t chunkId;
    uint32_t size;
    uint16_t wFormatTag;
    uint16_t wChannels;
    uint32_t dwSamplesPerSec;
    uint32_t dwAvgBytesPerSec;
    uint16_t wBlockAlign;
    uint16_t wBitsPerSamp;
};

struct __attribute__((packed)) DataHeader
{
    uint32_t chunkId;
    uint32_t size;
};

#endif  // SOUND_PROCESSOR_DATASTRUCTS_H