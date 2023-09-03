#pragma once

#include "kat/engine.hpp"
#include <unordered_set>

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

        // THIS SHOULD REALLY BE THE LAST THING CALLED IN THE MAIN LOOP
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

        namespace internal {
            inline std::unordered_set<int> keysPressedThisUpdate;
            inline std::unordered_set<int> keysPressedLastUpdate;

            void performUpdate();
        }
    }
}