#include "th/triggerhappy.hpp"

namespace th {

}

int main() {

    kat::Window::create(kat::Window::Config{
        "Window", kat::Window::Fullscreen{0, false}
    });

    while (kat::gbl::activeWindow->isOpen()) {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        kat::gbl::activeWindow->update();
    }

    kat::gbl::cleanup();
    return EXIT_SUCCESS;
}