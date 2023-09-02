#include "graphics.hpp"

namespace kat::graphics {
    void clear(const glm::vec4 &color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void polygonMode(PolygonMode mode) {
        glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(mode));
    }
}