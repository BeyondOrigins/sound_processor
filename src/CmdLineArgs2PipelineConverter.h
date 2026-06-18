#ifndef SOUND_PROCESSOR_CMDLINEARGS2PIPELINECONVERTER_H
#define SOUND_PROCESSOR_CMDLINEARGS2PIPELINECONVERTER_H
#include "ArgsParser.h"
#include "IFilter.h"
#include "Pipeline.h"
#include <map>
#include <string>
#include <vector>

class CmdLineArgs2PipelineConverter
{
public:
    using FilterProducer = IFilter* (*)(const FilterDescriptor&);
    CmdLineArgs2PipelineConverter() = default;

public:
    void addFilterProducer(const std::string& filterName,
                           FilterProducer producer);
    FilterProducer getFilterProducer(const std::string& filterName) const;
    Pipeline
    createPipeline(const std::vector<FilterDescriptor>& descriptors) const;

private:
    std::map<std::string, FilterProducer> _producers;
};

#endif  // SOUND_PROCESSOR_CMDLINEARGS2PIPELINECONVERTER_H