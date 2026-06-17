#ifndef SOUND_PROCESSOR_PIPELINE_H
#define SOUND_PROCESSOR_PIPELINE_H

#include "IFilter.h"
#include <vector>

class Pipeline
{
public:
    Pipeline() = default;
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;
    Pipeline(Pipeline&& other) noexcept : _filters(std::move(other._filters))
    {
        other._filters.clear();
    }
    Pipeline& operator=(Pipeline&& other) noexcept
    {
        if (this != &other)
        {
            for (const IFilter* filter : _filters)
                delete filter;
            _filters = std::move(other._filters);
            other._filters.clear();
        }
        return *this;
    }
    ~Pipeline()
    {
        for (const IFilter* filter : _filters)
            delete filter;
    }

    bool apply(Waveform* data) const;
public:
    size_t getFiltersNumber() const { return _filters.size(); }
    IFilter* operator[](size_t index) const { return _filters[index]; }
    void addFilter(IFilter* filter) { _filters.push_back(filter); }

private:
    std::vector<IFilter*> _filters;
};


#endif //SOUND_PROCESSOR_PIPELINE_H