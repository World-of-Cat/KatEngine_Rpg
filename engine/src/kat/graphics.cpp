#include "graphics.hpp"

namespace kat {
    std::shared_ptr<GraphicsContext> GraphicsContext::create() {
        gbl::activeGraphicsContext = std::shared_ptr<GraphicsContext>(new GraphicsContext());
        return gbl::activeGraphicsContext;
    }

    GraphicsContext::~GraphicsContext() {
    }

    GraphicsContext::GraphicsContext() {

    }
}