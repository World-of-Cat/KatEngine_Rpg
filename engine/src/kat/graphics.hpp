#pragma once

#include "kat/engine.hpp"
#include "kat/os.hpp"

namespace kat::graphics {
    void clear(const glm::vec4& color);

    enum class PolygonMode : GLenum {
        Fill = GL_FILL,
        Line = GL_LINE,
        Point = GL_POINT
    };

    void polygonMode(PolygonMode mode);
}