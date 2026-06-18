#ifndef PROJECT_ARGSPARSER_H
#define PROJECT_ARGSPARSER_H
#include <sstream>
#include <string>
#include <vector>

struct FilterDescriptor
{
    std::string filterName;
    std::vector<std::string> args;

    FilterDescriptor() = default;

    FilterDescriptor(std::string name, std::vector<std::string> arguments)
        : filterName(std::move(name)), args(std::move(arguments))
    {
    }

    explicit FilterDescriptor(const std::string& filterString)
    {
        std::istringstream stream(filterString);
        stream >> filterName;
        std::string tmp;
        while(stream >> tmp)
            args.push_back(tmp);
    }
};

class ArgsParser
{
public:
    enum class Result
    {
        ok,
        noArgs,
        badArgs,
    };

    ArgsParser() = default;

public:
    Result parse(int argc, char* argv[]);
    static void help();

public:
    [[nodiscard]] const std::string& getInFileName() const
    {
        return _inputFileName;
    }
    [[nodiscard]] const std::string& getOutFileName() const
    {
        return _outputFileName;
    }
    [[nodiscard]] const std::vector<FilterDescriptor>& getFilters() const
    {
        return _filters;
    }

private:
    std::string _inputFileName;
    std::string _outputFileName;
    std::vector<FilterDescriptor> _filters;
};

#endif  // PROJECT_ARGSPARSER_H