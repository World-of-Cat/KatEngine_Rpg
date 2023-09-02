#pragma once

namespace kat::util {
    constexpr glm::vec4 rgba8888(unsigned int v) {
        float r = float((v >> 24) & 0xff) / 255.0f;
        float g = float((v >> 16) & 0xff) / 255.0f;
        float b = float((v >> 8) & 0xff) / 255.0f;
        float a = float(v & 0xff) / 255.0f;
        return { r, g, b, a };
    };
}