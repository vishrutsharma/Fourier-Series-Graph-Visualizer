#include "WaveComposer.h"

void WaveComposer::AddHarmonic(std::unique_ptr<CC_Graph::Harmonics> harmonic)
{
    if (harmonic)
    {
        m_harmonics.push_back(std::move(harmonic));
    }
}

void WaveComposer::RemoveHarmonic(std::size_t index)
{
    if (index < m_harmonics.size())
    {
        m_harmonics.erase(m_harmonics.begin() + index);
    }
}

void WaveComposer::Clear()
{
    m_harmonics.clear();
}

const std::vector<std::unique_ptr<CC_Graph::Harmonics>> &WaveComposer::GetHarmonics() const
{
    return m_harmonics;
}

std::vector<std::unique_ptr<CC_Graph::Harmonics>> &WaveComposer::GetHarmonics()
{
    return m_harmonics;
}
