#ifndef SOUND_PROCESSOR_GENERATORFILTERS_H
#define SOUND_PROCESSOR_GENERATORFILTERS_H

#include "IFilter.h"

class AbstractGeneratorFilter : public IFilter
{
public:
    AbstractGeneratorFilter() = default;

    virtual ~AbstractGeneratorFilter() = default;
protected:
    double _durationMs;
};


#endif //SOUND_PROCESSOR_GENERATORFILTERS_H