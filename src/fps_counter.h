#pragma once

#include <chrono>
#include <array>
#include <cstddef>
#include <numeric>

// Lightweight, header-only FPS counter with fixed-size ring buffer to avoid allocations.
// Usage: create an instance, call frame() once per frame, then read getFPS() or getFrameTimeMs().
class FpsCounter
{
public:
    // number of samples used to smooth the FPS value (default 60)
    explicit FpsCounter(std::size_t sampleCount = 60)
        : m_sampleCount(sampleCount > 0 ? sampleCount : 1),
          m_index(0),
          m_filled(0),
          m_lastTime(std::chrono::high_resolution_clock::now())
    {
        m_deltas.resize(m_sampleCount);
        for (auto &d : m_deltas) d = 0.0;
    }

    // Call once per frame. Returns the instantaneous frame time in seconds.
    double frame()
    {
        using clock = std::chrono::high_resolution_clock;
        auto now = clock::now();
        std::chrono::duration<double> dt = now - m_lastTime;
        m_lastTime = now;

        double seconds = dt.count();
        if (seconds < 1e-12) seconds = 1e-12; // avoid division by zero

        m_deltas[m_index] = seconds;
        m_index = (m_index + 1) % m_sampleCount;
        if (m_filled < m_sampleCount) ++m_filled;

        return seconds;
    }

    // Smoothed FPS over the configured sample window.
    double getFPS() const
    {
        double avgSec = getAverageSeconds();
        return avgSec > 0.0 ? (1.0 / avgSec) : 0.0;
    }

    // Smoothed frame time in milliseconds.
    double getFrameTimeMs() const
    {
        return getAverageSeconds() * 1000.0;
    }

    // Reset internal state.
    void reset()
    {
        for (auto &d : m_deltas) d = 0.0;
        m_index = 0;
        m_filled = 0;
        m_lastTime = std::chrono::high_resolution_clock::now();
    }

private:
    double getAverageSeconds() const
    {
        if (m_filled == 0) return 0.0;
        double sum = 0.0;
        for (std::size_t i = 0; i < m_filled; ++i) sum += m_deltas[i];
        return sum / static_cast<double>(m_filled);
    }

    std::size_t m_sampleCount;
    std::size_t m_index;
    std::size_t m_filled;
    std::vector<double> m_deltas;
    std::chrono::high_resolution_clock::time_point m_lastTime;
};
