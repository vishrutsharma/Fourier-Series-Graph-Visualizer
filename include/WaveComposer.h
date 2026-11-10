#pragma once

#include "CoreComponents.h"
#include <cstddef>
#include <vector>
#include <memory>

class WaveComposer
{
private:
    std::vector<std::unique_ptr<CC_Graph::Harmonics>> m_harmonics;

public:
    void AddHarmonic(std::unique_ptr<CC_Graph::Harmonics> harmonic);
    void RemoveHarmonic(std::size_t index);
    void Clear();

    const std::vector<std::unique_ptr<CC_Graph::Harmonics>> &GetHarmonics() const;
    std::vector<std::unique_ptr<CC_Graph::Harmonics>> &GetHarmonics();
};
