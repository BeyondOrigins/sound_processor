#include "GeneratorFilters.h"
#include "Waveform.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

namespace {
constexpr double K_SAMPLE_RATE = 44100.0;
constexpr double K_PI = 3.14159265358979323846;
constexpr double K_MAX_AMPLITUDE = 32767.0;

int16_t toSample(double value)
{
    value = std::round(value);
    constexpr double LOW = std::numeric_limits<int16_t>::min();
    constexpr double HIGH = std::numeric_limits<int16_t>::max();
    return static_cast<int16_t>(std::clamp(value, LOW, HIGH));
}
}  // namespace

bool AbstractGeneratorFilter::apply(Waveform* sound)
{
    if(sound == nullptr || !isValid())
        return false;

    const auto COUNT =
        static_cast<size_t>(std::round(_durationMs * K_SAMPLE_RATE / 1000.0));

    std::vector<int16_t>& data = sound->getData();
    data.assign(COUNT, 0);
    for(size_t i = 0; i < COUNT; ++i)
    {
        const double TIME = static_cast<double>(i) / K_SAMPLE_RATE;
        data[i] = toSample(sampleValue(TIME));
    }
    sound->syncHeader();
    return true;
}

double SinGenFilter::sampleValue(double time) const
{
    return K_MAX_AMPLITUDE * std::sin(2.0 * K_PI * _frequencyHz * time);
}

double AmGenFilter::sampleValue(double time) const
{
    double envelope =
        1.0 + _depth * std::sin(2.0 * K_PI * _modulationHz * time);
    double carrier = std::sin(2.0 * K_PI * _carrierHz * time);
    return _amplitude * K_MAX_AMPLITUDE * envelope * carrier;
}

double FmGenFilter::sampleValue(double time) const
{
    const double PHASE = 2.0 * K_PI * _carrierHz * time +
                         (_deviationHz / _modulationHz) *
                             std::sin(2.0 * K_PI * _modulationHz * time);
    return _amplitude * K_MAX_AMPLITUDE * std::sin(PHASE);
}