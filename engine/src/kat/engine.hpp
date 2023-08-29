#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace kat {
    // Forward Definitions
    class Window;



    // Global State
    namespace gbl {
        inline std::shared_ptr<Window> activeWindow;

        void cleanup();
    }
}