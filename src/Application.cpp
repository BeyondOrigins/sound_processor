#include "Application.h"

#include "FilterProducers.h"
#include "Pipeline.h"
#include "Waveform.h"
#include "WavIStream.h"
#include "WavOStream.h"

#include <iostream>

void Application::configure()
{
    _converter.addFilterProducer("ampl",        FilterProducers::amplFilterCreator);
    _converter.addFilterProducer("normalize",   FilterProducers::normFilterCreator);
    _converter.addFilterProducer("silence",     FilterProducers::silenceFilterCreator);
    _converter.addFilterProducer("timestretch", FilterProducers::timeStretchFilterCreator);
    _converter.addFilterProducer("lowpass",     FilterProducers::lowpassFilterCreator);
    _converter.addFilterProducer("generator",   FilterProducers::generatorFilterCreator);
}

int Application::run(int argc, char* argv[])
{
    const ArgsParser::Result RESULT = _parser.parse(argc, argv);
    if (RESULT == ArgsParser::Result::noArgs)
    {
        ArgsParser::help();
        return 0;
    }
    if (RESULT == ArgsParser::Result::badArgs)
    {
        std::cerr << "Invalid arguments." << std::endl;
        ArgsParser::help();
        return 1;
    }

    Waveform waveform;
    if (!_parser.getInFileName().empty())
    {
        WavIStream input(_parser.getInFileName());
        input >> waveform;
    }

    Pipeline pipeline = _converter.createPipeline(_parser.getFilters());
    pipeline.apply(&waveform);

    if (!_parser.getOutFileName().empty())
    {
        WavOStream output(_parser.getOutFileName());
        output << waveform;
    }
    else
    {
        std::cerr << "No output file specified (-o); result not saved." << std::endl;
    }
    return 0;
}