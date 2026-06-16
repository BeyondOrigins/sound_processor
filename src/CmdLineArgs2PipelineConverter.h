#ifndef SOUND_PROCESSOR_CMDLINEARGS2PIPELINECONVERTER_H
#define SOUND_PROCESSOR_CMDLINEARGS2PIPELINECONVERTER_H
#include <map>
#include "IFilter.h"
#include "ArgsParser.h"


class CmdLineArgs2PipelineConverter
{
public:
    using FilterProducer = IFilter* (*) (const ArgsParser::FilterDescriptor&);
private:
    std::map<const char*, FilterProducer> _producers;
};


#endif //SOUND_PROCESSOR_CMDLINEARGS2PIPELINECONVERTER_H