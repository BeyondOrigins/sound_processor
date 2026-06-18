#ifndef SOUND_PROCESSOR_FILTERPRODUCERS_H
#define SOUND_PROCESSOR_FILTERPRODUCERS_H

#include "ArgsParser.h"
#include "IFilter.h"

namespace FilterProducers {
IFilter* amplFilterCreator(const FilterDescriptor& descriptor);
IFilter* normFilterCreator(const FilterDescriptor& descriptor);
IFilter* silenceFilterCreator(const FilterDescriptor& descriptor);
IFilter* timeStretchFilterCreator(const FilterDescriptor& descriptor);
IFilter* lowpassFilterCreator(const FilterDescriptor& descriptor);
IFilter* generatorFilterCreator(const FilterDescriptor& descriptor);
}  // namespace FilterProducers

#endif  // SOUND_PROCESSOR_FILTERPRODUCERS_H