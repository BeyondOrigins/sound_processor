#include "ModifyingFilters.h"
#include "Waveform.h"
#include <limits>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <cstdlib>

double samplesToMilliseconds(const WavInfo& info, size_t samples)
{
    return round(static_cast<double>(samples) / info.fmt.dwSamplesPerSec * 1000);
}

size_t millisecondsToSamples(const WavInfo& info, double amount)
{
    return round(amount * static_cast<double>(info.fmt.dwSamplesPerSec) / 1000);
}

bool AmplFilter::apply(Waveform* sound)
{
    if(_ampl< 0 || sound->getData().empty())
        return false;
    double max = std::numeric_limits<int16_t>::max();
    double min = std::numeric_limits<int16_t>::min();
    for(auto& sample: sound->getData())
    {
        double newSample = _ampl * sample;
        sample = std::clamp(newSample, min, max);
    }
    return true;
}

bool NormFilter::apply(Waveform* sound)
{
    if(!(0.0 <= _peak && _peak <= 1.0))
        return false;
    if(sound->getData().empty())
        return true;
    int16_t currentPeak = *std::max_element(
        sound->getData().begin(), sound->getData().end(),
        [](double lhs, double rhs) { return std::abs(lhs) < std::abs(rhs); });
    currentPeak = std::clamp(currentPeak, static_cast<int16_t>(-32767),
                             static_cast<int16_t>(32767));
    if(currentPeak == 0.0)
        return true;
    double scale = _peak * 32767 / std::abs(currentPeak);
    for(auto& sample: sound->getData())
        sample *= scale;
    return true;
}

bool SilenceFilter::apply(Waveform* sound)
{
    if(_unit != "ms" && _unit != "sec" || _start < 0 || _end < _start)
        return false;
    if(_start == _end)
        return true;
    double startMs = (_unit == "sec") ? _start * 1000.0 : _start;
    double endMs   = (_unit == "sec") ? _end   * 1000.0 : _end;
    size_t startUnits = millisecondsToSamples(sound->getDataInfo(), startMs);
    size_t endUnits   = millisecondsToSamples(sound->getDataInfo(), endMs);
    size_t dataSize = sound->getDataSize();
    if(startUnits > dataSize)
    {
        size_t delta = startUnits - dataSize;
        startUnits -= delta;
        endUnits -= delta;
    }
    size_t silenceUnits = endUnits - startUnits;
    sound->getData().insert(sound->getData().begin() + startUnits, silenceUnits,
                            0);
    sound->syncHeader();
    return true;
}

bool TimeStretchFilter::apply(Waveform* sound)
{
    if (_factor <= 0.0)
        return false;
    size_t size = sound->getDataSize();
    if (!size)
        return false;
    size_t newDataSize = round(size*_factor);
    std::vector<int16_t>& data = sound->getData();
    std::vector<int16_t> src = sound->getData();
    data.resize(newDataSize);
    for (size_t i = 0; i < newDataSize; ++i)
    {
        double real = i / _factor;
        auto pos  = static_cast<size_t>(real);
        double frac = real - pos;
        double v = src[pos] * (1.0 - frac);
        v += (pos + 1 < size ? src[pos + 1] : src[pos]) * frac;
        data[i] = static_cast<int16_t>(std::round(v));
    }
    sound->syncHeader();
    return true;
}

bool LowpassFilter::apply(Waveform* sound)
{
    if (_windowSize < 1 || _windowSize % 2 == 0 || \
        sound->getData().empty())
        return false;
    size_t bias = _windowSize / 2;
    size_t size = sound->getDataSize();
    std::vector<int16_t> src = sound->getData();
    std::vector<int16_t>& data = sound->getData();
    for (size_t i = 0; i < size; ++i)
    {
        int32_t sum = 0;
        for (int j = static_cast<int>(i) - static_cast<int>(bias); j <= static_cast<int>(i + bias)
             ; ++j)
        {
            int idx = std::clamp(j, 0, static_cast<int>(size) - 1);
            sum += src[idx];
        }
        sum /= static_cast<int32_t>(_windowSize);
        data[i] = static_cast<int16_t>(sum);
    }
    return true;
}
