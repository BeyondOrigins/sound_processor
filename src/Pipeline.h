#ifndef SOUND_PROCESSOR_PIPELINE_H
#define SOUND_PROCESSOR_PIPELINE_H

#include "IFilter.h"
#include <vector>

class Pipeline
{
public:
    Pipeline() = default;

    ~Pipeline()
    {
        for (IFilter* filter : _filters)
            delete filter;
    }

    bool apply(Waveform* data);
public:
    size_t getFiltersNumber() const { return _filters.size(); }
    IFilter* operator[](size_t index) const { return _filters[index]; }
    void addFilter(IFilter* filter) { _filters.push_back(filter); }

private:
    std::vector<IFilter*> _filters;
};


#endif //SOUND_PROCESSOR_PIPELINE_H