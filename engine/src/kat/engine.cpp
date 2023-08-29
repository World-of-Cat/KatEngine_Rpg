#include "kat/engine.hpp"

namespace kat {
    void gbl::cleanup() {
        gbl::activeWindow = nullptr;
    }
}