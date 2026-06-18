#ifndef SOUND_PROCESSOR_GENERATORFILTERS_H
#define SOUND_PROCESSOR_GENERATORFILTERS_H

#include "IFilter.h"

#include <iostream>

class AbstractGeneratorFilter: public IFilter
{
public:
    AbstractGeneratorFilter() = delete;
    explicit AbstractGeneratorFilter(double durationMs): _durationMs(durationMs)
    {
    }
    ~AbstractGeneratorFilter() override = default;

    bool apply(Waveform* sound) override;

protected:
    virtual double sampleValue(double t) const = 0;
    virtual bool isValid() const { return _durationMs >= 0.0; }

    double _durationMs;
};

class SinGenFilter: public AbstractGeneratorFilter
{
public:
    SinGenFilter() = delete;
    SinGenFilter(const SinGenFilter&) = default;
    SinGenFilter(SinGenFilter&&) = default;
    SinGenFilter& operator=(const SinGenFilter&) = default;
    SinGenFilter& operator=(SinGenFilter&&) = default;
    SinGenFilter(double durationMs, double frequencyHz)
        : AbstractGeneratorFilter(durationMs), _frequencyHz(frequencyHz)
    {
    }
    ~SinGenFilter() override = default;

    double getFrequencyHz() const { return _frequencyHz; }
    void failMessage() const override
    {
        std::cerr << "Could not apply Sin generator" << std::endl;
    }

protected:
    double sampleValue(double time) const override;
    bool isValid() const override
    {
        return _durationMs >= 0.0 && _frequencyHz >= 0.0;
    }

    double _frequencyHz;
};

class AmGenFilter: public AbstractGeneratorFilter
{
public:
    AmGenFilter() = delete;
    AmGenFilter(const AmGenFilter&) = default;
    AmGenFilter(AmGenFilter&&) = default;
    AmGenFilter& operator=(const AmGenFilter&) = default;
    AmGenFilter& operator=(AmGenFilter&&) = default;
    AmGenFilter(double durationMs, double amplitude, double carrierHz,
                double modulationHz, double depth)
        : AbstractGeneratorFilter(durationMs), _amplitude(amplitude),
          _carrierHz(carrierHz), _modulationHz(modulationHz), _depth(depth)
    {
    }
    ~AmGenFilter() override = default;

    double getAmplitude() const { return _amplitude; }
    double getCarrierHz() const { return _carrierHz; }
    double getModulationHz() const { return _modulationHz; }
    double getDepth() const { return _depth; }
    void failMessage() const override
    {
        std::cerr << "Could not apply AM generator" << std::endl;
    }

protected:
    double sampleValue(double time) const override;
    bool isValid() const override
    {
        return _durationMs >= 0.0 && _amplitude >= 0.0 && _amplitude <= 1.0 &&
               _carrierHz >= 0.0 && _modulationHz >= 0.0 && _depth >= 0.0 &&
               _depth <= 1.0;
    }

    double _amplitude, _carrierHz, _modulationHz, _depth;
};

class FmGenFilter: public AbstractGeneratorFilter
{
public:
    FmGenFilter() = delete;
    FmGenFilter(const FmGenFilter&) = default;
    FmGenFilter(FmGenFilter&&) = default;
    FmGenFilter& operator=(const FmGenFilter&) = default;
    FmGenFilter& operator=(FmGenFilter&&) = default;
    FmGenFilter(double durationMs, double amplitude, double carrierHz,
                double modulationHz, double deviationHz)
        : AbstractGeneratorFilter(durationMs), _amplitude(amplitude),
          _carrierHz(carrierHz), _modulationHz(modulationHz),
          _deviationHz(deviationHz)
    {
    }
    ~FmGenFilter() override = default;

    double getAmplitude() const { return _amplitude; }
    double getCarrierHz() const { return _carrierHz; }
    double getModulationHz() const { return _modulationHz; }
    double getDeviationHz() const { return _deviationHz; }
    void failMessage() const override
    {
        std::cerr << "Could not apply FM generator" << std::endl;
    }

protected:
    double sampleValue(double time) const override;
    bool isValid() const override
    {
        return _durationMs >= 0.0 && _amplitude >= 0.0 && _amplitude <= 1.0 &&
               _carrierHz >= 0.0 && _modulationHz > 0.0 && _deviationHz >= 0.0;
    }

    double _amplitude, _carrierHz, _modulationHz, _deviationHz;
};

#endif  // SOUND_PROCESSOR_GENERATORFILTERS_H