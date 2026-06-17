#ifndef SOUND_PROCESSOR_FILTERPRODUCERS_H
#define SOUND_PROCESSOR_FILTERPRODUCERS_H

#include "IFilter.h"
#include "ArgsParser.h"

namespace FilterProducers
{
IFilter* amplFilterCreator(const FilterDescriptor& descriptor);
IFilter* normFilterCreator(const FilterDescriptor& descriptor);
IFilter* silenceFilterCreator(const FilterDescriptor& descriptor);
IFilter* timeStretchFilterCreator(const FilterDescriptor& descriptor);
IFilter* lowpassFilterCreator(const FilterDescriptor& descriptor);
IFilter* generatorFilterCreator(const FilterDescriptor& descriptor);
}

#endif  // SOUND_PROCESSOR_FILTERPRODUCERS_H