#pragma once

#include <unordered_map>
#include <any>
#include <filesystem>
#include <vector>
#include <glm/glm.hpp>

namespace kat::rpg {

    struct CustomPropertyTable {
        std::unordered_map<std::string, std::any> properties;
    };

    struct Tile {
        unsigned int globalId;

        unsigned int tilesetId;

        glm::uvec2 size;

        // TODO: Wang Tiles Autotiling

        CustomPropertyTable properties;
    };
}