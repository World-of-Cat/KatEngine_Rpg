#pragma once

#include <string>
#include <glm/glm.hpp>
#include <optional>
#include <vector>
#include <variant>

namespace kat::util::tiled {
    struct Frame {
        unsigned int tileId;
        unsigned int duration;
    };

    struct Animation {
        std::vector<Frame> frames;
    };

    enum class ObjectAlignment {
        Unspecified, /* unspecified */
        TopLeft, /* topleft */
        Top, /* top */
        TopRight, /* topright */
        Left, /* left */
        Center, /* center */
        Right, /* right */
        BottomLeft, /* bottomleft */
        Bottom, /* bottom */
        BottomRight /* bottomright */
    };

    enum class TileRenderSize {
        Tile, /* tile */
        Grid /* grid */
    };

    enum class FillMode {
        Stretch, /* stretch */
        PreserveAspectFit /* preserve-aspect-fit */
    };

    enum class Orientation {
        Orthogonal, /* orthogonal */
        Isometric, /* isometric */
    };

    struct Grid {
        Orientation orientation = Orientation::Orthogonal; // orientation
        glm::uvec2 cellSize; // width, height
    };

    enum class ImageFormat {
        Unspecified, // not present
        Png, /* png */
        Gif, /* gif */
        Jpg, /* jpg */
        Bmp, /* bmp */
    };

    struct Image {
        ImageFormat format = ImageFormat::Unspecified; // format
        std::string source; // source

        // transparency color. optional. hex representation rgb888. may or may not begin with #
        std::optional<std::string> trans; // trans
        std::optional<glm::uvec2> size; // width, height
    };

    struct ObjectGroup {

    };

    struct Properties {

    };

    struct Tile {
        unsigned int id;
        std::string type = ""; // type
        float probability; // probability
        glm::uvec2 position = {0, 0}; // x, y
        std::optional<glm::uvec2> size; // override on size. if individual tile, defaults to image size
        std::optional<Animation> animation; // <animation>
        std::optional<Image> image; // <image>
        std::optional<ObjectGroup> objectGroup; // <objectgroup>
        std::optional<Properties> properties; // <properties>
    };

    struct WangColor {

    };

    struct WangSet {
        std::string name; // name
        std::string class_ = ""; // class
        unsigned int tile; // tile

        std::optional<Properties> properties; // <properties>

        kat::util::bounded_array<WangColor, 254>;
    };

    struct WangSets {
        std::vector<WangSet> wangSets;
    };


    struct Transformations {
        bool hflip = false; // hflip
        bool vflip = false; // vflip
        bool rotate = false; // rotate
        bool preferUntransformed = false; // preferuntransformed
    };

    struct Tileset {
        std::string name; // name
        std::string class_ = ""; // class
        glm::uvec2 tileSize; // tilewidth, tileheight
        unsigned int spacing = 0; // spacing
        unsigned int margin = 0; // margin
        unsigned int tileCount; // tilecount
        unsigned int columns; // columns
        ObjectAlignment objectAlignment = ObjectAlignment::Unspecified; // objectalignment
        TileRenderSize tileRenderSize = TileRenderSize::Tile; // tilerendersize
        FillMode fillMode = FillMode::Stretch; // fillemode
        glm::uvec2 tileOffset = {0, 0}; // <tileoffset x=? y=?>
        std::optional<Grid> grid; // <grid>
    };

    struct InternalTileset {
        unsigned int firstGid;
        std::variant<Tileset, std::string> tileset; // Either the source parameter (ref to external) or internally contained data.
    };
}