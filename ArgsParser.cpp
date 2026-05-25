#include "ArgsParser.h"
#include <iostream>

ArgsParser::Result ArgsParser::parse(int argc,
                                     char *argv[]) { // parse arguments
    if (argc == 1)
        return Result::noArgs;

    return Result::ok;
}

void ArgsParser::help() { // help command
    std::cout << "Command format:" << std::endl;
    std::cout << "{command} [-i path_to_input_file] [-o path_to_output_file]" << std::endl;
    std::cout << "[-f {filter_name1} [filter1_param1] [filter1_param2]...]" << std::endl;
    std::cout << "[-f {filter_name2} [filter2_param1] [filter2_param2]...]..." << std::endl;
}