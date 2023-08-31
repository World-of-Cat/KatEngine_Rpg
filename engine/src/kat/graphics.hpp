#pragma once

#include "kat/engine.hpp"
#include "kat/os.hpp"

namespace kat {


    class GraphicsContext {
    public:

        static std::shared_ptr<GraphicsContext> create();

        ~GraphicsContext();

    private:

        GraphicsContext();
    };
}