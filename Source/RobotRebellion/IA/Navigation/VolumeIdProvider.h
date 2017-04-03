#pragma once

#include "../../Tool/IsSingleton.h"


/**
*
*/
class ROBOTREBELLION_API VolumeIdProvider : private IsSingleton<VolumeIdProvider>
{
    GENERATED_USING_FROM_IsSingleton(VolumeIdProvider);

private:

    int m_count;
    VolumeIdProvider() : m_count{}
    {}
public:
    ~VolumeIdProvider()
    {
        reset();
    }
    int getNextId()
    {
        // we need to start at 0
        int returnValue = m_count++;
        return returnValue;
    }
    void reset()
    {
        m_count = 0;
    }
};