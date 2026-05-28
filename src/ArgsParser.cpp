#include "ArgsParser.h"
#include <iostream>
#include <CLI/CLI.hpp>

ArgsParser::Result ArgsParser::parse(int argc,
                                     char *argv[]) { // parse arguments
    if (argc == 1)
        return Result::noArgs;
    CLI::App app {"Sound processor"};
    std::string inputFileName;
    std::string outputFileName;
    std::vector<std::string> filtersStrings;
    app.add_option("-i", inputFileName, "Input file");
    app.add_option("-o", outputFileName, "Output file");
    app.add_option("-f", filtersStrings, "Add filter");
    try {
        app.parse(argc, argv);
    }
    catch (const std::exception &_) {
        return Result::badArgs;
    }
    _inputFileName = inputFileName;
    _outputFileName = outputFileName;
    _filters = std::vector<FilterDescriptor>();
    for (auto &filterString : filtersStrings) {
        FilterDescriptor desc(filterString);
        _filters.push_back(desc);
    }
    return Result::ok;
}

void ArgsParser::help() { // help command
    std::cout << "Command format:" << std::endl;
    std::cout << "{command} [-i path_to_input_file] [-o path_to_output_file]" << std::endl;
    std::cout << "[-f {filter_name1} [filter1_param1] [filter1_param2]...]" << std::endl;
    std::cout << "[-f {filter_name2} [filter2_param1] [filter2_param2]...]..." << std::endl;
}