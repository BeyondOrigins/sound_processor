#ifndef SOUND_PROCESSOR_GENERATORFILTERS_H
#define SOUND_PROCESSOR_GENERATORFILTERS_H

#include "IFilter.h"

class AbstractGeneratorFilter : public IFilter
{
public:
    AbstractGeneratorFilter() = delete;
    AbstractGeneratorFilter(double durationMs) : _durationMs(durationMs) {}
    virtual ~AbstractGeneratorFilter() = default;
protected:
    double _durationMs;
};

class SinGenFilter : public AbstractGeneratorFilter
{
public:
    SinGenFilter() = delete;
    SinGenFilter(const SinGenFilter&) = default;
    SinGenFilter(SinGenFilter&&) = default;
    SinGenFilter& operator=(const SinGenFilter&) = default;
    SinGenFilter& operator=(SinGenFilter&&) = default;
    SinGenFilter(double durationMs, double frequencyHz) : AbstractGeneratorFilter(durationMs), _frequencyHz(frequencyHz) {}
    ~SinGenFilter() override = default;
public:
    bool apply(Waveform* sound) override;
protected:
    double _frequencyHz;
};


#endif //SOUND_PROCESSOR_GENERATORFILTERS_H