#pragma once

#include "kat/engine.hpp"

namespace kat {


    class Window : std::enable_shared_from_this<Window> {
    public:

        struct Fullscreen {
            uint32_t monitorIndex = 0;
            bool exclusive = false;
        };

        using Mode = std::variant<glm::uvec2, Fullscreen>;

        struct Config {
            std::string title = "Window";
            Mode mode = Fullscreen{0, false};
        };

        static std::shared_ptr<Window> create(const Config& config);

        ~Window();

        bool isOpen() const;

        void update();

        glm::ivec2 getSize() const;

        GLFWwindow* operator*() const noexcept;
        [[nodiscard]] GLFWwindow* getHandle() const noexcept;

        bool getKey(int key);

    private:
        explicit Window(const Config &config);

        GLFWwindow *m_Window;
    };

    namespace input {
        bool isKeyPressed(int key);
    }
}