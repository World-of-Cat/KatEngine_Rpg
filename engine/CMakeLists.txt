cmake_minimum_required(VERSION 3.24)
project(KatEngine VERSION 0.1.0)

add_library(KatEngine src/kat/engine.cpp src/kat/engine.hpp
        src/kat/os.cpp
        src/kat/os.hpp
        src/kat/graphics.cpp
        src/kat/graphics.hpp
        src/kat/graphics/texture.cpp
        src/kat/graphics/texture.hpp
        src/kat/graphics/mesh.cpp
        src/kat/graphics/mesh.hpp
        src/kat/graphics/shader.cpp
        src/kat/graphics/shader.hpp
        src/kat/graphics/render_target.cpp
        src/kat/graphics/render_target.hpp
        src/kat/util/camera.cpp
        src/kat/util/camera.hpp
        src/kat/util/interfaces.hpp
        src/kat/graphics/colors.hpp
        src/kat/util/math.hpp
        src/kat/util/clock.hpp
        src/kat/graphics/sprite.cpp
        src/kat/graphics/sprite.hpp
        src/kat/util/transform_stack.hpp)
target_include_directories(KatEngine PUBLIC src/)
target_link_libraries(KatEngine PUBLIC glfw glad::glad glm::glm stb::stb spdlog::spdlog eventpp::eventpp)

add_library(KatEngine::KatEngine ALIAS KatEngine)
