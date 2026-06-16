#include "ModifyingFilters.h"
#include "Waveform.h"
#include <limits>
#include <numeric>

double samplesToMilliseconds(const WavInfo& info, size_t samples)
{
    return samples * info.fmt.dwSamplesPerSec * 1000;
}

size_t millisecondsToSamples(const WavInfo& info, double amount)
{
    return round(amount / info.fmt.dwSamplesPerSec * 1000);
}

bool AmplFilter::apply(Waveform* sound)
{
    if(sound->getData().empty())
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
    if(currentPeak == 0.0)
        return true;
    double scale = _peak * 32767 / currentPeak;
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
    size_t startUnits = millisecondsToSamples(sound->getDataInfo(), _start);
    size_t endUnits = millisecondsToSamples(sound->getDataInfo(), _end);
    if(_unit == "sec")
    {
        startUnits /= 1000;
        endUnits /= 1000;
    }
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
    if (newDataSize > size)
        data.resize(newDataSize);
    for (size_t i = 0; i < newDataSize; ++i)
    {
        size_t pos = i / _factor;
        data[i] = data[pos]*(1-_factor);
        if (pos == size-1)
            data[i] += data[pos]*_factor;
        else
            data[i] += data[pos+1]*_factor;
    }
    if (newDataSize < size)
        data.resize(newDataSize);
    sound->syncHeader();
    return true;
}

bool LowpassFilter::apply(Waveform* sound)
{
    if (_windowSize <= 1 || _windowSize % 2 == 0 || \
        sound->getData().empty())
        return false;
    size_t bias = _windowSize / 2;
    size_t size = sound->getDataSize();
    for (size_t i = 0; i < size; ++i)
    {
        int32_t sum = 0;
        for (int j = i-bias; j <= i+bias; ++j)
        {
            int idx = std::clamp(j, 0, static_cast<int>(size) - 1);
            sum += sound->getData()[idx];
        }
        sum /= _windowSize;
        sound->getData()[i] = static_cast<int16_t>(sum);
    }
    return true;
}
