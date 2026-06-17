#include "FilterProducers.h"

#include "ModifyingFilters.h"
#include "GeneratorFilters.h"

#include <stdexcept>
#include <string>

namespace
{

double toDouble(const std::string& token, const std::string& what)
{
    try
    {
        size_t pos = 0;
        const double value = std::stod(token, &pos);
        if (pos != token.size())
            throw std::invalid_argument("trailing characters");
        return value;
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument("Bad number for " + what + ": '" + token + "'");
    }
}

size_t toSize(const std::string& token, const std::string& what)
{
    try
    {
        size_t pos = 0;
        const int value = std::stoi(token, &pos);
        if (pos != token.size() || value < 0)
            throw std::invalid_argument("not a non-negative integer");
        return static_cast<size_t>(value);
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument("Bad integer for " + what + ": '" + token + "'");
    }
}

void requireArgs(const FilterDescriptor& descriptor, size_t expected)
{
    if (descriptor.args.size() != expected)
        throw std::invalid_argument(
            "Filter '" + descriptor.filterName + "': expected " +
            std::to_string(expected) + " parameter(s), got " +
            std::to_string(descriptor.args.size()));
}
}  // namespace

IFilter* FilterProducers::amplFilterCreator(const  FilterDescriptor& descriptor)
{
    if (descriptor.filterName != "ampl")
        throw std::invalid_argument("Wrong fd for ampl filter producer");
    requireArgs(descriptor, 1);
    return new AmplFilter(toDouble(descriptor.args[0], "ampl factor"));
}

IFilter* FilterProducers::normFilterCreator(const  FilterDescriptor& descriptor)
{
    if (descriptor.filterName != "normalize")
        throw std::invalid_argument("Wrong fd for normalize filter producer");
    if (descriptor.args.empty())
        return new NormFilter();  // peak по умолчанию = 1
    requireArgs(descriptor, 1);
    return new NormFilter(toDouble(descriptor.args[0], "normalize peak"));
}

IFilter* FilterProducers::silenceFilterCreator(const  FilterDescriptor& descriptor)
{
    if (descriptor.filterName != "silence")
        throw std::invalid_argument("Wrong fd for silence filter producer");
    requireArgs(descriptor, 3);
    const std::string& unit = descriptor.args[0];
    if (unit != "ms" && unit != "sec")
        throw std::invalid_argument("silence: unit must be 'ms' or 'sec'");
    return new SilenceFilter(unit,
                             toDouble(descriptor.args[1], "silence start"),
                             toDouble(descriptor.args[2], "silence end"));
}

IFilter* FilterProducers::timeStretchFilterCreator(const FilterDescriptor& descriptor)
{
    if (descriptor.filterName != "timestretch")
        throw std::invalid_argument("Wrong fd for timestretch filter producer");
    requireArgs(descriptor, 1);
    return new TimeStretchFilter(toDouble(descriptor.args[0], "timestretch factor"));
}

IFilter* FilterProducers::lowpassFilterCreator(const FilterDescriptor& descriptor)
{
    if (descriptor.filterName != "lowpass")
        throw std::invalid_argument("Wrong fd for lowpass filter producer");
    requireArgs(descriptor, 1);
    return new LowpassFilter(toSize(descriptor.args[0], "lowpass window_size"));
}

IFilter* FilterProducers::generatorFilterCreator(const FilterDescriptor& descriptor)
{
    if (descriptor.filterName != "generator")
        throw std::invalid_argument("Wrong fd for generator filter producer");
    if (descriptor.args.empty())
        throw std::invalid_argument("generator: missing kind (sin/am/fm)");

    const std::string& kind = descriptor.args[0];
    if (kind == "sin")
    {
        requireArgs(descriptor, 3);
        return new SinGenFilter(toDouble(descriptor.args[2], "duration_ms"),
                                toDouble(descriptor.args[1], "frequency_hz"));
    }
    if (kind == "am")
    {
        requireArgs(descriptor, 6);
        return new AmGenFilter(toDouble(descriptor.args[5], "duration_ms"),
                               toDouble(descriptor.args[1], "amplitude"),
                               toDouble(descriptor.args[2], "carrier_hz"),
                               toDouble(descriptor.args[3], "modulation_hz"),
                               toDouble(descriptor.args[4], "depth"));
    }
    if (kind == "fm")
    {
        requireArgs(descriptor, 6);
        return new FmGenFilter(toDouble(descriptor.args[5], "duration_ms"),
                               toDouble(descriptor.args[1], "amplitude"),
                               toDouble(descriptor.args[2], "carrier_hz"),
                               toDouble(descriptor.args[3], "modulation_hz"),
                               toDouble(descriptor.args[4], "deviation_hz"));
    }
    throw std::invalid_argument("generator: unknown kind '" + kind + "'");
}