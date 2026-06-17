#include <catch2/catch_test_macros.hpp>

#include "ArgsParser.h"
#include "CmdLineArgs2PipelineConverter.h"
#include "FilterProducers.h"
#include "Pipeline.h"
#include "Waveform.h"
#include "WavIStream.h"
#include "WavOStream.h"
#include "ModifyingFilters.h"
#include "GeneratorFilters.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

namespace
{

struct Cmd
{
    std::vector<std::string> store;
    std::vector<char*> ptrs;
    explicit Cmd(std::initializer_list<const char*> tokens)
    {
        for (const char* t : tokens) store.emplace_back(t);
        for (std::string& s : store) ptrs.push_back(s.data());
    }
    int argc() { return static_cast<int>(ptrs.size()); }
    char** argv() { return ptrs.data(); }
};

WavInfo makeWavInfo(size_t sampleCount)
{
    WavInfo info{};
    std::memcpy(&info.riff.chunkId, "RIFF", 4);
    std::memcpy(&info.riff.waveId,  "WAVE", 4);
    std::memcpy(&info.fmt.chunkId,  "fmt ", 4);
    std::memcpy(&info.data.chunkId, "data", 4);
    info.fmt.size             = 16;
    info.fmt.wFormatTag       = 1;
    info.fmt.wChannels        = 1;
    info.fmt.dwSamplesPerSec  = 44100;
    info.fmt.dwAvgBytesPerSec = 44100 * 2;
    info.fmt.wBlockAlign      = 2;
    info.fmt.wBitsPerSamp     = 16;
    info.data.size = static_cast<uint32_t>(sampleCount * sizeof(int16_t));
    info.riff.size = static_cast<uint32_t>(sampleCount * sizeof(int16_t) + 36);
    return info;
}

Waveform makeWaveform(std::vector<int16_t> samples)
{
    WavInfo info = makeWavInfo(samples.size());
    return Waveform(info, std::move(samples));
}

CmdLineArgs2PipelineConverter makeConverter()
{
    CmdLineArgs2PipelineConverter c;
    c.addFilterProducer("ampl",        FilterProducers::amplFilterCreator);
    c.addFilterProducer("normalize",   FilterProducers::normFilterCreator);
    c.addFilterProducer("silence",     FilterProducers::silenceFilterCreator);
    c.addFilterProducer("timestretch", FilterProducers::timeStretchFilterCreator);
    c.addFilterProducer("lowpass",     FilterProducers::lowpassFilterCreator);
    c.addFilterProducer("generator",   FilterProducers::generatorFilterCreator);
    return c;
}

int maxAbs(const std::vector<int16_t>& v)
{
    int m = 0;
    for (int16_t s : v)
    {
        const int a = (s < 0) ? -static_cast<int>(s) : static_cast<int>(s);
        if (a > m) m = a;
    }
    return m;
}
}  // namespace

TEST_CASE("ArgsParser keeps -f groups separate", "[parser]")
{
    ArgsParser parser;
    Cmd cmd{"sp", "-i", "in.wav", "-o", "out.wav",
            "-f", "ampl", "0.8",
            "-f", "silence", "sec", "0.2", "0.4",
            "-f", "generator", "sin", "440", "1000"};
    REQUIRE(parser.parse(cmd.argc(), cmd.argv()) == ArgsParser::Result::ok);
    REQUIRE(parser.getInFileName() == "in.wav");
    REQUIRE(parser.getOutFileName() == "out.wav");

    const auto& filters = parser.getFilters();
    REQUIRE(filters.size() == 3);
    REQUIRE(filters[0].filterName == "ampl");
    REQUIRE(filters[0].args == std::vector<std::string>{"0.8"});
    REQUIRE(filters[1].filterName == "silence");
    REQUIRE(filters[1].args == std::vector<std::string>{"sec", "0.2", "0.4"});
    REQUIRE(filters[2].filterName == "generator");
    REQUIRE(filters[2].args == std::vector<std::string>{"sin", "440", "1000"});
}

TEST_CASE("ArgsParser handles edge cases", "[parser]")
{
    ArgsParser parser;

    SECTION("no args")
    {
        Cmd cmd{"sp"};
        REQUIRE(parser.parse(cmd.argc(), cmd.argv()) == ArgsParser::Result::noArgs);
    }
    SECTION("negative parameter is not a flag")
    {
        Cmd cmd{"sp", "-f", "ampl", "-0.5"};
        REQUIRE(parser.parse(cmd.argc(), cmd.argv()) == ArgsParser::Result::ok);
        REQUIRE(parser.getFilters().at(0).args == std::vector<std::string>{"-0.5"});
    }
    SECTION("missing value after flag")
    {
        Cmd cmd{"sp", "-i"};
        REQUIRE(parser.parse(cmd.argc(), cmd.argv()) == ArgsParser::Result::badArgs);
    }
    SECTION("unknown token")
    {
        Cmd cmd{"sp", "garbage"};
        REQUIRE(parser.parse(cmd.argc(), cmd.argv()) == ArgsParser::Result::badArgs);
    }
}

TEST_CASE("FilterDescriptor parses a string without duplicating last token", "[parser]")
{
    FilterDescriptor d("silence sec 0.2 0.4 ");
    REQUIRE(d.filterName == "silence");
    REQUIRE(d.args == std::vector<std::string>{"sec", "0.2", "0.4"});

    FilterDescriptor direct("ampl", {"0.8"});
    REQUIRE(direct.filterName == "ampl");
    REQUIRE(direct.args == std::vector<std::string>{"0.8"});
}

// ------------------------ Producers / Converter ------------------------

TEST_CASE("Converter builds a pipeline from descriptors", "[converter]")
{
    CmdLineArgs2PipelineConverter conv = makeConverter();
    std::vector<FilterDescriptor> descriptors{
        FilterDescriptor("ampl", {"0.8"}),
        FilterDescriptor("lowpass", {"3"}),
        FilterDescriptor("generator", {"sin", "440", "1000"}),
    };
    Pipeline pipeline = conv.createPipeline(descriptors);
    REQUIRE(pipeline.getFiltersNumber() == 3);
}

TEST_CASE("Converter and producers report errors", "[converter]")
{
    CmdLineArgs2PipelineConverter conv = makeConverter();

    SECTION("unknown filter")
    {
        REQUIRE_THROWS(conv.createPipeline({FilterDescriptor("nope", {})}));
    }
    SECTION("wrong number of parameters")
    {
        REQUIRE_THROWS(conv.createPipeline({FilterDescriptor("ampl", {"1", "2"})}));
    }
    SECTION("non-numeric parameter")
    {
        REQUIRE_THROWS(conv.createPipeline({FilterDescriptor("ampl", {"abc"})}));
    }
    SECTION("unknown generator kind")
    {
        REQUIRE_THROWS(conv.createPipeline({FilterDescriptor("generator", {"triangle", "1"})}));
    }
    SECTION("getFilterProducer returns nullptr for unknown name")
    {
        REQUIRE(conv.getFilterProducer("doesNotExist") == nullptr);
    }
}

TEST_CASE("generator producer dispatches by kind", "[converter]")
{
    CmdLineArgs2PipelineConverter conv = makeConverter();
    REQUIRE(conv.createPipeline({FilterDescriptor("generator", {"sin", "440", "10"})}).getFiltersNumber() == 1);
    REQUIRE(conv.createPipeline({FilterDescriptor("generator", {"am", "0.9", "440", "5", "0.5", "10"})}).getFiltersNumber() == 1);
    REQUIRE(conv.createPipeline({FilterDescriptor("generator", {"fm", "0.8", "440", "5", "100", "10"})}).getFiltersNumber() == 1);
}

// ------------------------------ Pipeline ------------------------------

TEST_CASE("Pipeline owns filters and is movable", "[pipeline]")
{
    Pipeline pipeline;
    pipeline.addFilter(new AmplFilter(1.0));
    pipeline.addFilter(new AmplFilter(2.0));
    REQUIRE(pipeline.getFiltersNumber() == 2);

    Pipeline moved = std::move(pipeline);
    REQUIRE(moved.getFiltersNumber() == 2);
    REQUIRE(pipeline.getFiltersNumber() == 0);
}

TEST_CASE("Pipeline applies filters in order", "[pipeline]")
{
    Pipeline pipeline;
    pipeline.addFilter(new SinGenFilter(10.0, 440.0));
    pipeline.addFilter(new AmplFilter(0.0));
    Waveform w;
    REQUIRE(pipeline.apply(&w));
    REQUIRE(w.getDataSize() == 441);
    REQUIRE(maxAbs(w.getData()) == 0);
}

TEST_CASE("AmplFilter scales and clamps", "[filters][ampl]")
{
    SECTION("plain scaling")
    {
        Waveform w = makeWaveform({100, -100, 200});
        REQUIRE(AmplFilter(2.0).apply(&w));
        REQUIRE(w.getData() == std::vector<int16_t>{200, -200, 400});
    }
    SECTION("clamps to int16 range")
    {
        Waveform hi = makeWaveform({20000});
        AmplFilter(2.0).apply(&hi);
        REQUIRE(hi.getData()[0] == 32767);

        Waveform lo = makeWaveform({-20000});
        AmplFilter(2.0).apply(&lo);
        REQUIRE(lo.getData()[0] == -32768);
    }
}

TEST_CASE("NormFilter brings peak to peak*32767", "[filters][normalize]")
{
    Waveform w = makeWaveform({1000, -2000, 500});
    REQUIRE(NormFilter(1.0).apply(&w));
    REQUIRE(maxAbs(w.getData()) == 32767);
}

TEST_CASE("SilenceFilter inserts zeros and shifts the tail", "[filters][silence]")
{
    Waveform w = makeWaveform({1, 2, 3, 4});
    REQUIRE(SilenceFilter("sec", 0.0, 0.001).apply(&w));
    REQUIRE(w.getDataSize() == 4 + 44);
    REQUIRE(w.getData()[0] == 0);
    REQUIRE(w.getData()[44] == 1);
}

TEST_CASE("TimeStretchFilter changes length by factor", "[filters][timestretch]")
{
    SECTION("stretch x2")
    {
        Waveform w = makeWaveform({0, 100, 200, 300, 400, 500, 600, 700, 800, 900});
        REQUIRE(TimeStretchFilter(2.0).apply(&w));
        REQUIRE(w.getDataSize() == 20);
        REQUIRE(w.getData()[0] == 0);
    }
    SECTION("compress x0.5")
    {
        Waveform w = makeWaveform({0, 100, 200, 300, 400, 500, 600, 700, 800, 900});
        REQUIRE(TimeStretchFilter(0.5).apply(&w));
        REQUIRE(w.getDataSize() == 5);
    }
    SECTION("non-positive factor is rejected")
    {
        Waveform w = makeWaveform({1, 2, 3});
        REQUIRE(TimeStretchFilter(-1.0).apply(&w) == false);
        REQUIRE(w.getDataSize() == 3);
    }
}

TEST_CASE("LowpassFilter smooths with edge clamping", "[filters][lowpass]")
{
    SECTION("window 1 is identity")
    {
        Waveform w = makeWaveform({1, 2, 3, 4, 5});
        REQUIRE(LowpassFilter(1).apply(&w));
        REQUIRE(w.getData() == std::vector<int16_t>{1, 2, 3, 4, 5});
    }
    SECTION("constant signal stays constant")
    {
        Waveform w = makeWaveform({500, 500, 500, 500, 500});
        REQUIRE(LowpassFilter(3).apply(&w));
        REQUIRE(w.getData() == std::vector<int16_t>{500, 500, 500, 500, 500});
    }
    SECTION("even window is rejected")
    {
        Waveform w = makeWaveform({1, 2, 3});
        REQUIRE(LowpassFilter(2).apply(&w) == false);
    }
}



TEST_CASE("Generators produce the right length and start at zero", "[generators]")
{
    SECTION("sin")
    {
        Waveform w;
        REQUIRE(SinGenFilter(10.0, 440.0).apply(&w));  // round(10*44100/1000) = 441
        REQUIRE(w.getDataSize() == 441);
        REQUIRE(w.getData()[0] == 0);             // sin(0) = 0
        REQUIRE(maxAbs(w.getData()) >= 32000);
    }
    SECTION("am and fm have the same length rule")
    {
        Waveform am;
        REQUIRE(AmGenFilter(10.0, 0.9, 440.0, 5.0, 0.5).apply(&am));
        REQUIRE(am.getDataSize() == 441);

        Waveform fm;
        REQUIRE(FmGenFilter(10.0, 0.8, 440.0, 5.0, 100.0).apply(&fm));
        REQUIRE(fm.getDataSize() == 441);
    }
    SECTION("invalid parameters and null are rejected")
    {
        Waveform w;
        REQUIRE(SinGenFilter(-1.0, 440.0).apply(&w) == false);
        REQUIRE(SinGenFilter(10.0, 440.0).apply(nullptr) == false);
    }
}


TEST_CASE("WAV write then read returns identical samples", "[wav]")
{
    std::vector<int16_t> samples;
    for (int i = 0; i < 100; ++i)
        samples.push_back(static_cast<int16_t>(i * 100 - 5000));

    const std::string path = "sp_test_roundtrip.wav";
    Waveform original(makeWavInfo(samples.size()), samples);
    {
        WavOStream out(path);
        out << original;
    }
    Waveform restored;
    {
        WavIStream in(path);
        in >> restored;
    }
    REQUIRE(restored.getDataSize() == samples.size());
    REQUIRE(restored.getData() == samples);

    std::ifstream check(path, std::ios::binary | std::ios::ate);
    REQUIRE(static_cast<long>(check.tellg()) == 44 + static_cast<long>(samples.size()) * 2);
    check.close();
    std::remove(path.c_str());
}

TEST_CASE("Reading a non-WAV file is rejected", "[wav]")
{
    const std::string path = "sp_not_a_wav.bin";
    {
        std::ofstream f(path, std::ios::binary);
        f << "this is definitely not a wav header payload";
    }
    REQUIRE_THROWS([&] { WavIStream in(path); }());
    std::remove(path.c_str());
}