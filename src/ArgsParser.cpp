#include "ArgsParser.h"
#include <iostream>
#include <string>

namespace {
constexpr std::string FLAGS[] = {"-i", "-o", "-f"};
bool isFlag(const std::string& token)
{
    return std::ranges::find(FLAGS, std::string_view(token)) != std::end(FLAGS);
}
}  // namespace

ArgsParser::Result ArgsParser::parse(int argc, char* argv[])
{
    if(argc == 1)
        return Result::noArgs;

    _inputFileName.clear();
    _outputFileName.clear();
    _filters.clear();

    for(int i = 1; i < argc; ++i)
    {
        const std::string ARG = argv[i];

        if(ARG == "-i")
        {
            if(++i >= argc)
                return Result::badArgs;
            _inputFileName = argv[i];
        }
        else if(ARG == "-o")
        {
            if(++i >= argc)
                return Result::badArgs;
            _outputFileName = argv[i];
        }
        else if(ARG == "-f")
        {
            if(++i >= argc)
                return Result::badArgs;
            std::string name = argv[i];
            std::vector<std::string> filterArgs;
            while(i + 1 < argc && !isFlag(argv[i + 1]))
                filterArgs.push_back(argv[++i]);
            _filters.emplace_back(std::move(name), std::move(filterArgs));
        }
        else
        {
            return Result::badArgs;
        }
    }
    return Result::ok;
}

void ArgsParser::help()
{  // help command
    std::cout << "Command format:" << std::endl;
    std::cout << "{command} [-i path_to_input_file] [-o path_to_output_file]"
              << std::endl;
    std::cout << "[-f {filter_name1} [filter1_param1] [filter1_param2]...]"
              << std::endl;
    std::cout << "[-f {filter_name2} [filter2_param1] [filter2_param2]...]..."
              << std::endl;
}