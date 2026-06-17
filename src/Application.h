#ifndef SOUND_PROCESSOR_APPLICATION_H
#define SOUND_PROCESSOR_APPLICATION_H

#include "ArgsParser.h"
#include "CmdLineArgs2PipelineConverter.h"

class Application
{
public:
    Application() { configure(); }

    int run(int argc, char* argv[]);

private:
    void configure();

    ArgsParser _parser;
    CmdLineArgs2PipelineConverter _converter;
};

#endif  // SOUND_PROCESSOR_APPLICATION_H