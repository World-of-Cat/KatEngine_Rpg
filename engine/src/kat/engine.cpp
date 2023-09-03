#include "kat/engine.hpp"
#include "kat/os.hpp"

#include <fstream>
#include <ranges>
#include <algorithm>
#include "kat/graphics/sprite.hpp"
#include "kat/util/transform_stack.hpp"

namespace kat {
    void gbl::cleanup() {
        gbl::appEvents.dispatch(AppEvent::Cleanup);
        gbl::activeWindow = nullptr;
    }

    std::string util::readFile(const std::filesystem::path &path) {
        std::ifstream f(path);
        std::string s((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        return s;
    }

    bool util::isWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    std::string util::strip(const std::string &in) {
        return rstrip(lstrip(in));
    }

    std::string util::lstrip(const std::string &in) {
        size_t l = 0;
        while (l < in.size() && isWhitespace(in[l])) l++;
        if (l == 0) return in;
        return in.substr(l);
    }

    std::string util::rstrip(const std::string &in) {
        size_t l = in.size();
        while (l > 0 && isWhitespace(in[l])) l--;
        if (l == 0) return in;
        return in.substr(0, l);

    }

    void gbl::setup() {
        gbl::appEvents.appendListener(AppEvent::Initialize, kat::Sprite::init);
        gbl::appEvents.appendListener(AppEvent::Cleanup, kat::Sprite::cleanup);

        gbl::appEvents.appendListener(AppEvent::Cleanup, [](){ kat::transform::unravel(true); });
        gbl::appEvents.appendListener(AppEvent::Update, [](){ kat::transform::unravel(false); });
    }
}