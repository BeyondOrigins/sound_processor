#ifndef PROJECT_ARGSPARSER_H
#define PROJECT_ARGSPARSER_H
#include <vector>
#include <sstream>

class ArgsParser {
public:
    enum class Result {
        ok,
        noArgs,
        badArgs,
    };

    struct FilterDescriptor {
        std::string filterName;
        std::vector<std::string> args;

        explicit FilterDescriptor(const std::string& filterString)
        {
            std::stringstream ss(filterString);
            std::string tmp;
            ss >> filterName;
            while (!ss.eof())
            {
                ss >> tmp;
                args.push_back(tmp);
            }
        }
    };

    ArgsParser() = default;

public:
    Result parse(int argc, char *argv[]);
    static void help();
public:
    [[nodiscard]] const std::string& getInFileName() const { return _inputFileName; }
    [[nodiscard]] const std::string& getOutFileName() const { return _outputFileName; }
    [[nodiscard]] const std::vector<FilterDescriptor>& getFilters() const { return _filters; }
private:
    std::string _inputFileName;
    std::string _outputFileName;
    std::vector<FilterDescriptor> _filters;
};


#endif //PROJECT_ARGSPARSER_H