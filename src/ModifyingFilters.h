#ifndef SOUND_PROCESSOR_MODIFYINGFILTERS_H
#define SOUND_PROCESSOR_MODIFYINGFILTERS_H

#include "IFilter.h"

#include <iostream>
#include <ostream>
#include <string>

class AmplFilter: public IFilter
{
public:
    AmplFilter() = delete;
    AmplFilter(const AmplFilter&) = default;
    AmplFilter& operator=(const AmplFilter&) = default;
    AmplFilter(AmplFilter&&) = default;
    AmplFilter& operator=(AmplFilter&&) = default;
    explicit AmplFilter(double ampl): _ampl{ampl} {}
    ~AmplFilter() override = default;

public:
    bool apply(Waveform* sound) override;
    double getAmpl() const { return _ampl; }
    void failMessage() const override
    {
        std::cerr << "Could not apply Ampl filter" << std::endl;
    }

protected:
    double _ampl;
};

class NormFilter: public IFilter
{
public:
    NormFilter(): _peak(1.0) {}
    NormFilter(const NormFilter&) = default;
    NormFilter& operator=(const NormFilter&) = default;
    NormFilter(NormFilter&&) = default;
    NormFilter& operator=(NormFilter&&) = default;
    explicit NormFilter(double peak): _peak{peak} {}
    ~NormFilter() override = default;

public:
    bool apply(Waveform* sound) override;
    double getPeak() const { return _peak; }
    void failMessage() const override
    {
        std::cerr << "Could not apply Normalize filter" << std::endl;
    }

protected:
    double _peak;
};

class SilenceFilter: public IFilter
{
public:
    SilenceFilter() = delete;
    SilenceFilter(const SilenceFilter&) = default;
    SilenceFilter& operator=(const SilenceFilter&) = default;
    SilenceFilter(SilenceFilter&&) = default;
    SilenceFilter& operator=(SilenceFilter&&) = default;
    ~SilenceFilter() override = default;
    SilenceFilter(const std::string& unit, double start, double end)
        : _unit(unit), _start(start), _end(end)
    {
    }

public:
    bool apply(Waveform* sound) override;
    std::string getUnit() const { return _unit; }
    double getStart() const { return _start; }
    double getEnd() const { return _end; }
    void failMessage() const override
    {
        std::cerr << "Could not apply Silence filter" << std::endl;
    }

protected:
    std::string _unit;
    double _start;
    double _end;
};

class TimeStretchFilter: public IFilter
{
public:
    TimeStretchFilter() = delete;
    TimeStretchFilter(const TimeStretchFilter&) = default;
    TimeStretchFilter& operator=(const TimeStretchFilter&) = default;
    TimeStretchFilter(TimeStretchFilter&&) = default;
    TimeStretchFilter& operator=(TimeStretchFilter&&) = default;
    ~TimeStretchFilter() override = default;
    explicit TimeStretchFilter(double factor): _factor{factor} {}

public:
    bool apply(Waveform* sound) override;
    double getFactor() const { return _factor; }
    void failMessage() const override
    {
        std::cerr << "Could not apply TimeStretch filter" << std::endl;
    }

protected:
    double _factor;
};

class LowpassFilter: public IFilter
{
public:
    LowpassFilter() = delete;
    LowpassFilter(const LowpassFilter&) = default;
    LowpassFilter& operator=(const LowpassFilter&) = default;
    LowpassFilter(LowpassFilter&&) = default;
    LowpassFilter& operator=(LowpassFilter&&) = default;
    ~LowpassFilter() override = default;
    explicit LowpassFilter(size_t windowSize): _windowSize{windowSize} {}

public:
    bool apply(Waveform* sound) override;
    size_t getWindowSize() const { return _windowSize; }
    void failMessage() const override
    {
        std::cerr << "Could not apply Lowpass filter" << std::endl;
    }

protected:
    size_t _windowSize;
};

#endif  // SOUND_PROCESSOR_MODIFYINGFILTERS_H