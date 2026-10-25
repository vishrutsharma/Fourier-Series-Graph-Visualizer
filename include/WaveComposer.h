#pragma once

#include "CoreComponents.h"
#include <vector>
#include <memory>

using namespace std;

class WaveComposer
{
private:
    vector<unique_ptr<CC_Graph::Harmonics>> m_harmonics;

public:
    void AddHarmonic(unique_ptr<CC_Graph::Harmonics>);
};