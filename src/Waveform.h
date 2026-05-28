#ifndef SOUND_PROCESSOR_WAVEFORM_H
#define SOUND_PROCESSOR_WAVEFORM_H
#include <cstdint>
#include <vector>


class Waveform
{
public:
    Waveform();
private:
    std::vector<int16_t> _mSamples;

};


#endif //SOUND_PROCESSOR_WAVEFORM_H