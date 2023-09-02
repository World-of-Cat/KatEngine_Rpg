#pragma once

#include "kat/engine.hpp"
#include "glm/gtc/constants.hpp"

#include <concepts>

namespace kat::util {

    template<typename T>
    concept position_t = (std::same_as<T, glm::vec<2, typename T::value_type, glm::precision::defaultp>> ||
            std::same_as<T, glm::vec<3, typename T::value_type, glm::precision::defaultp>>) &&
            std::floating_point<typename T::value_type>;

    template<position_t T>
    class IPositionable {
    public:
        inline virtual void setPosition(const T& pos) {
            m_Position = pos;
        };

        inline virtual void move(const T& delta) {
            m_Position += delta;
        };

        inline virtual const T& getPosition() const noexcept {
            return m_Position;
        };

    protected:

        T m_Position = glm::zero<T>();
    };
}