#include "WaveComposer.h"

void WaveComposer::AddHarmonic(unique_ptr<CC_Graph::Harmonics> harmonic)
{
    m_harmonics.push_back(std::move(harmonic));
}

CC_Graph::Harmonics WaveComposer::GetHarmonics()
{
    if (m_harmonics != null && m_harmonics.size > 0)
    {
        return m_harmonics;
    }
}