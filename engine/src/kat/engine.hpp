#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <eventpp/eventdispatcher.h>

#include <filesystem>
#include <string>
#include <vector>

namespace kat {
    // Forward Definitions
    class Window;

    enum AppEvent {
        Initialize, Cleanup, Update
    };

    // Global State
    namespace gbl {

        inline std::shared_ptr<Window> activeWindow;

        void cleanup();
        void setup(); // registers init/cleanup event handlers

        inline eventpp::EventDispatcher<AppEvent, void()> appEvents;
    }

    // Utility
    namespace util {
        std::string readFile(const std::filesystem::path& path);

        bool isWhitespace(char c);

        std::string strip(const std::string& in);
        std::string lstrip(const std::string& in);
        std::string rstrip(const std::string& in);
    }
}