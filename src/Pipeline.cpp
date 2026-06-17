#include "Pipeline.h"

#include "Waveform.h"

bool Pipeline::apply(Waveform* data) const
{
    for (IFilter* filter : _filters)
    {
        bool result = filter->apply(data);
        if (!result)
            filter->failMessage();
    }
    return true;
}