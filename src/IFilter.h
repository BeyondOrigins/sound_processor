#ifndef SOUND_PROCESSOR_IFILTER_H
#define SOUND_PROCESSOR_IFILTER_H
#include <cstdint>

class Waveform;

class IFilter
{
public:
    virtual ~IFilter() = default;
    virtual bool apply(Waveform* sound) = 0;
    virtual void failMessage() const = 0;
};

#endif  // SOUND_PROCESSOR_IFILTER_H