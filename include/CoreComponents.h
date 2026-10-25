#pragma once
#include <cmath>
#include "TimeManager.h"
#include <iostream>

namespace CC_Graph
{
    enum class HarmonicsType
    {
        NONE,
        SIN,
        COS,
        TAN
    };

    struct Harmonics
    {
        HarmonicsType funcType = HarmonicsType::NONE;

        float amplitude;
        float frequency;
        float phase;
        float offset;

        inline float Draw(float x)
        {
            float elapsedTime = TimeManager::GetInstance().GetTotalTime();
            switch (funcType)
            {
            case HarmonicsType::SIN:
                return amplitude * std::sin(frequency * x + phase + elapsedTime) + offset;
            case HarmonicsType::COS:
                return amplitude * std::cos(frequency * x + phase + elapsedTime) + offset;
            case HarmonicsType::TAN:
                return amplitude * std::tan(frequency * x + phase + elapsedTime) + offset;
            default:
                return 0.0f;
            }
        }
    };
}
