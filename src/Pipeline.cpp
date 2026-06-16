#include "Pipeline.h"

#include "Waveform.h"

#include <iostream>
#include <ostream>

bool Pipeline::apply(Waveform* data)
{
    for (IFilter* filter : _filters)
    {
        bool result = filter->apply(data);
        if (!result)
            filter->failMessage();
    }
    return true;
}