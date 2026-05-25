#ifndef PROJECT_ARGSPARSER_H
#define PROJECT_ARGSPARSER_H
#include <vector>

class ArgsParser {
public:
    enum class Result {
        ok,
        noArgs,
        badArgs,
    };

    struct FilterDescriptor {
        const char* filterName;
        std::vector<char*> args;

        FilterDescriptor(const char* filterName, const std::vector<char*>& args) :
            filterName(filterName), args(args) {}

    };

    ArgsParser() : _inputFileName(nullptr), _outputFileName(nullptr) {}

public:
    Result parse(int argc, char *argv[]);
    static void help();
public:
    [[nodiscard]] char* getInFileName() const { return _inputFileName; }
    [[nodiscard]] char* getOutFileName() const { return _outputFileName; }
private:
    char* _inputFileName;
    char* _outputFileName;
    std::vector<FilterDescriptor> _filters;
};


#endif //PROJECT_ARGSPARSER_H