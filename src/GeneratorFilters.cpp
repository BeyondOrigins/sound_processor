#include "GeneratorFilters.h"
#include "Waveform.h"

double samplesToMilliseconds(const WavInfo& info, size_t samples)
{
    return samples * info.fmt.dwSamplesPerSec * 1000;
}

size_t millisecondsToSamples(const WavInfo& info, double amount)
{
    return round(amount / info.fmt.dwSamplesPerSec * 1000);
}

bool SinGenFilter::apply(Waveform* sound)
{

    return true;
}