#include "CmdLineArgs2PipelineConverter.h"

#include <stdexcept>

void CmdLineArgs2PipelineConverter::addFilterProducer(
    const std::string& filterName, FilterProducer producer)
{
    _producers[filterName] = producer;
}

CmdLineArgs2PipelineConverter::FilterProducer
CmdLineArgs2PipelineConverter::getFilterProducer(
    const std::string& filterName) const
{
    const auto it = _producers.find(filterName);
    return it == _producers.end() ? nullptr : it->second;
}

Pipeline CmdLineArgs2PipelineConverter::createPipeline(
    const std::vector<FilterDescriptor>& descriptors) const
{
    Pipeline pipeline;
    for(const FilterDescriptor& descriptor: descriptors)
    {
        const FilterProducer PRODUCER =
            getFilterProducer(descriptor.filterName);
        if(PRODUCER == nullptr)
            throw std::invalid_argument("Unknown filter: " +
                                        descriptor.filterName);
        pipeline.addFilter(PRODUCER(descriptor));
    }
    return pipeline;
}