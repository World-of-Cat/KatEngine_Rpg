#pragma once

#include <chrono>
#include <concepts>
#include <type_traits>

namespace kat::util {
    class Clock {
    public:
        using clock = std::chrono::high_resolution_clock;
        using duration = std::chrono::duration<double, std::ratio<1>>;
        using time_point = std::chrono::time_point<clock, duration>;

        inline Clock() : m_LastFrame(clock::now() - duration(1.0 / 60.0)),
                         m_ThisFrame(clock::now()), m_DeltaTime(1.0 / 60.0) {};

        inline void tick() {
            m_FrameCounter++;
            m_LastFrame = m_ThisFrame;
            m_ThisFrame = clock::now();
            m_DeltaTime = m_ThisFrame - m_LastFrame;
        };

        [[nodiscard]] duration getDeltaTime() const noexcept {
            return m_DeltaTime;
        };

        [[nodiscard]] time_point getLastFrame() const noexcept {
            return m_LastFrame;
        };

        [[nodiscard]] time_point getThisFrame() const noexcept {
            return m_ThisFrame;
        };

    private:

        unsigned long long m_FrameCounter = 0;
        time_point m_LastFrame;
        time_point m_ThisFrame;
        duration m_DeltaTime;
    };
}